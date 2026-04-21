#include <Arduino.h>
#include <HardwareSerial.h>
#include "LoRaModule_esp.h"

#define RXLORA_RX 3
#define RXLORA_TX 2

#define TXLORA_RX 5
#define TXLORA_TX 4

#define RX_MODULE_ADDRESS 4
#define TX_MODULE_ADDRESS 1
#define RX_MODULE_BAND 915000000UL
#define TX_MODULE_BAND 928000000UL
#define LORA_NETWORK_ID 18

void parseReceivedPayload(const String& hexPayload);
void checkUserInput();
void send_command(String inputString, String address, LoRaModule& module);

LoRaModule rxLora(Serial1, RXLORA_RX, RXLORA_TX, RX_MODULE_ADDRESS);
LoRaModule txLora(Serial2, TXLORA_RX, TXLORA_TX, TX_MODULE_ADDRESS);

bool reporting_lock = false;
String lora_input = "";
String address = "";

void setup() {
  Serial.begin(115200);
  delay(100);

  if (!rxLora.begin()) {
    Serial.println("RX LoRa init failed");
  }
  if (!txLora.begin()) {
    Serial.println("TX LoRa init failed");
  }

  rxLora.configure(RX_MODULE_ADDRESS, RX_MODULE_BAND, LORA_NETWORK_ID);
  txLora.configure(TX_MODULE_ADDRESS, TX_MODULE_BAND, LORA_NETWORK_ID);
}

void loop() {
  String incomingHex;
  if (rxLora.receiveData(incomingHex)) {
    parseReceivedPayload(incomingHex);
  }
  delay(10);

  checkUserInput();

  static unsigned long lastRepeatMillis = 0;
  static bool flipState = false;

  if (lora_input != "" && address != "") {
    bool doSend = true;

    if (reporting_lock) {
      if (millis() - lastRepeatMillis >= 1000) {
        lastRepeatMillis = millis();
        flipState = !flipState;
        lora_input = flipState ? "fe00" : "8000";
        doSend = true;
      } else {
        doSend = false;
      }
    }

    if (doSend) {
      send_command(lora_input, address, txLora);
    }

    if (!reporting_lock) {
      lora_input = "";
      address = "";
    }
  }

  delay(10);
}

void parseReceivedPayload(const String& hexPayload) {
  const int byteLen = hexPayload.length() / 2;
  if (byteLen <= 0 || (hexPayload.length() % 2) != 0) {
    return;
  }

  uint8_t buffer[64];
  if (byteLen > static_cast<int>(sizeof(buffer))) {
    return;
  }

  for (int i = 0; i < byteLen; i++) {
    const String byteString = hexPayload.substring(i * 2, i * 2 + 2);
    buffer[i] = static_cast<uint8_t>(strtol(byteString.c_str(), NULL, 16));
  }

  Serial.write(0xAA);
  Serial.write(0x55);
  Serial.write(buffer, byteLen);
}

void checkUserInput() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n');
    userInput.trim();

    if (userInput.length() > 0) {
      const bool isRepeat = userInput.endsWith(":repeat");

      if (isRepeat) {
        const int colonIndex = userInput.indexOf(':');
        if (colonIndex != -1) {
          userInput = userInput.substring(0, colonIndex);
          reporting_lock = true;
          Serial.println("Repeat mode enabled");
        }
      } else {
        reporting_lock = false;
      }

      const int commaIndex = userInput.indexOf(",");
      if (commaIndex != -1) {
        lora_input = userInput.substring(0, commaIndex);
        address = userInput.substring(commaIndex + 1);
      } else {
        Serial.println("ERROR: Invalid format. Use: <command>,<address>");
      }
    }
  }
}

void send_command(String inputString, String address, LoRaModule& module) {
  const long addressInt = address.toInt();
  if (addressInt < 0 || addressInt > 255) {
    return;
  }

  module.sendData(static_cast<uint8_t>(addressInt), inputString);
  delay(100);
}
