// #include <Wire.h>
#include <HardwareSerial.h> // Provides 3x UART hardware serial capability
#include <Arduino.h>

#define I2C_DEV_ADDR 0x08

// LORA 1 AND 2 RESPECTIVELY

#define RXLORA_RX 3 // TX on LORA; ESP IS RECEIVING
#define RXLORA_TX 2 // RX on LORA; ESP IS TRANSMITTING

#define TXLORA_RX 5 // TX on LORA; ESP IS RECEIVING
#define TXLORA_TX 4 // RX on LORA; ESP IS TRANSMITTING

String sendATcommand(const char *toSend, unsigned long ms, HardwareSerial &name);
void parseReceivedMessage(String msg);
void checkUserInput();
void send_command(String lora_input, String address, HardwareSerial& name);

HardwareSerial RXLORA(1); // Hardware UART 1
HardwareSerial TXLORA(2); // Hardware UART 2

// lora_sender.ino

bool reporting_lock = false;
String lora_input = "";
String address = "";
unsigned long sendStartTime = 0;
bool waitingForReply = false;

String toJetson; // Initialize empty string that will eventually be sent to Jetson

/* void onRequest() {
  Wire.print(toJetson);
}

void onReceive(int len) {
  while (Wire.available()) {
    char c = Wire.read();
    if (c >= 32 && c <= 126) { 
      Serial.print(c);
    }
  }
} */

void setup() {
  Serial.begin(115200);
  delay(100);

  RXLORA.begin(115200, SERIAL_8N1, RXLORA_RX, RXLORA_TX); // LoRa 1 is RXLORA
  TXLORA.begin(115200, SERIAL_8N1, TXLORA_RX, TXLORA_TX); // LoRa 2 is TXLORA

  delay(1000);

  // initialize LoRa modules silently
  String res1 = sendATcommand("AT", 100, RXLORA);
  String res2 = sendATcommand("AT", 100, TXLORA);
  // Serial.print("1");

  // responses not logged

  // Configure this module as receiver (address 3)
  sendATcommand("AT+ADDRESS=4", 100, RXLORA);
  sendATcommand("AT+ADDRESS=1", 100, TXLORA);
  // Serial.print("2");
  delay(100);
  sendATcommand("AT+BAND=915000000", 100, RXLORA);
  sendATcommand("AT+BAND=928000000", 100, TXLORA);
  // Serial.print("3");
  delay(100);
  sendATcommand("AT+NETWORKID=18", 100, RXLORA);
  sendATcommand("AT+NETWORKID=18", 100, TXLORA);
  // Serial.print("4");
  delay(100);
  sendATcommand("AT+PARAMETER=11,9,4,24", 100, RXLORA);
  sendATcommand("AT+PARAMETER=11,9,4,24", 100, TXLORA);
  // Serial.print("5");
  delay(100);

  // setup complete
}

/* void loop() {
  if (RXLORA.available()){
    String incoming = RXLORA.readStringUntil('\n');
    Serial.print("Raw Lora: ");
    Serial.println(incoming);

    if (incoming.startsWith("+RCV")){
      parseAndSendToJetson(incoming);
    }
  }
} */

// lora_sender.ino

void loop() {
  // Check for incoming LoRa messages
  if (RXLORA.available()) {
    String incomingString = "";
    
    // Read with timeout
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
      if (RXLORA.available()) {
        char c = RXLORA.read();
        if (c == '\n') break;
        incomingString += c;
      }
    }
    
    if (incomingString.length() > 0) {
      if (incomingString.indexOf("+RCV=") != -1) {
        parseReceivedMessage(incomingString);
      }
    }
  }
  delay(10);

  checkUserInput();
  
  // Send message if we have input and address
  //  - when reporting_lock is *not* set we behave normally (one send per command)
  //  - when reporting_lock is set we toggle between two hex codes once per second
  static unsigned long lastRepeatMillis = 0;
  static bool flipState = false; // false -> 0x8000, true -> 0xFE00

  if (lora_input != "" && address != "") {
    bool doSend = true;

    if (reporting_lock) {
      // generate a new value once per second
      if (millis() - lastRepeatMillis >= 1000) {
        lastRepeatMillis = millis();
        flipState = !flipState;
        // override the user-supplied payload with our hexcodes
        lora_input = flipState ? "fe00" : "8000";
        doSend = true;
      } else {
        doSend = false;
      }
    }

    if (doSend) {
      send_command(lora_input, address, TXLORA);
    }

    // Clear after sending unless in repeat mode
    if (!reporting_lock) {
      lora_input = "";
      address = "";
    }
  }
  
  delay(10);
}

void parseReceivedMessage(String rawMessage) {
  // Remove +RCV= prefix
  int startIdx = rawMessage.indexOf("+RCV=");
  if (startIdx == -1) return;
  
  String data = rawMessage.substring(startIdx + 5);  // Skip "+RCV="
  
  // Parse comma-separated values
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);
  int fourthComma = data.indexOf(',', thirdComma + 1);
  
  if (firstComma == -1 || secondComma == -1) {
    // Serial.println("Parse error");
    return;
  }
  
  // Extract fields
  String senderAddr = data.substring(0, firstComma);
  String length = data.substring(firstComma + 1, secondComma);
  
  String message;
  String rssi = "N/A";
  String snr = "N/A";
  
  if (thirdComma != -1) {
    message = data.substring(secondComma + 1, thirdComma);
    if (fourthComma != -1) {
      rssi = data.substring(thirdComma + 1, fourthComma);
      snr = data.substring(fourthComma + 1);
    } else {
      rssi = data.substring(thirdComma + 1);
    }
  } else {
    message = data.substring(secondComma + 1);
  }
  
  // Convert hex string payload to raw bytes
  int byteLen = message.length() / 2;
  if (byteLen <= 0) return;

  uint8_t buffer[64];   // plenty of space (your packets are 20 bytes)

  for (int i = 0; i < byteLen; i++) {
    String byteString = message.substring(i * 2, i * 2 + 2);
    buffer[i] = (uint8_t) strtol(byteString.c_str(), NULL, 16);
  }

  // Send raw binary directly to PC over USB
  Serial.write(0xAA);
  Serial.write(0x55);
  Serial.write(buffer, byteLen);
}

/* void sendConfirmation(String targetAddr) {
  String msg = "OKAY";
  int len = msg.length();

  char atCommand[50];
  snprintf(atCommand, sizeof(atCommand), "AT+SEND=%d,%d,%s",
           targetAddr.toInt(), len, msg.c_str());

  Serial.print("Sending confirmation to ");
  Serial.println(targetAddr);

  sendATcommand(atCommand, 1000, RXLORA);
} */


void checkUserInput() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n');
    userInput.trim();
    
    if (userInput.length() > 0) {
      // Check if the input ends with ":repeat"
      bool isRepeat = userInput.endsWith(":repeat");
      
      if (isRepeat) {
        int colonIndex = userInput.indexOf(':');
        if (colonIndex != -1) {
          userInput = userInput.substring(0, colonIndex);
          reporting_lock = true;
          Serial.println("Repeat mode enabled");
        }
      } else {
        reporting_lock = false;
      }
      
      // Parse command and address
      int commaIndex = userInput.indexOf(",");
      if (commaIndex != -1) {
        lora_input = userInput.substring(0, commaIndex);
        address = userInput.substring(commaIndex + 1);
        
        Serial.print("Command: ");
        Serial.print(lora_input);
        Serial.print(" | Address: ");
        Serial.println(address);
      } else {
        Serial.println("ERROR: Invalid format. Use: <command>,<address>");
      }
    }
  }
}

void send_command(String inputString, String address, HardwareSerial &name) {
  int len = inputString.length();
  int addressInt = address.toInt();
  
  if (addressInt < 0 || addressInt > 65535) {
    // Serial.println("ERROR: Invalid address");
    return;
  }
  
  // Serial.print("Preparing to send: '");
  // Serial.print(inputString);
  // Serial.print("' (length: ");
  // Serial.print(len);
  // Serial.print(") to address ");
  // Serial.println(addressInt);
  
  // Build the AT command
  // Format: AT+SEND=<address>,<length>,<data>
  char atCommand[100];
  
  snprintf(atCommand, sizeof(atCommand), "AT+SEND=%d,%d,%s", 
           addressInt, len, inputString.c_str());
  
  // Serial.print("AT Command: ");
  // Serial.println(atCommand);
  
  sendStartTime = millis();
  waitingForReply = true;
  String response = sendATcommand(atCommand, 100, name);
  
  // Check if send was successful
  if (response.indexOf("OK") != -1) {
    // Serial.println("✓ Send successful");

  } else if (response.indexOf("+ERR") != -1) {
    // Serial.println("✗ Send failed - Error response");
    if (!reporting_lock) {  
      lora_input = "";
      address = "";
    }
  } else {
    // Serial.println("? Unknown response");
  }
  
  delay(100);  // Small delay between sends
}

// On call, parses received data and sends to Jetson

void parseAndSendToJetson(String rcvData){
  int firstComma = rcvData.indexOf(',');
  int secondComma = rcvData.indexOf(',', firstComma + 1);
  int thirdComma = rcvData.indexOf(',', secondComma + 1);

  String payload = rcvData.substring(secondComma + 1, thirdComma);
  
  // Serial.print("Payload detected: ");
  // Serial.println(payload);
  toJetson = payload.c_str();
  // Serial.print(toJetson);
}

// AT Command Sender

String sendATcommand(const char *toSend, unsigned long ms, HardwareSerial &name) {
  name.println(toSend);
  unsigned long start = millis();
  String res = "";
  while (millis() - start < ms) {
    if (name.available()) res += (char)name.read();
  }
  // Serial.println(res);
  return res;
}
