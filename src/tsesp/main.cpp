// Receiver: LoRa2 -> MCU2 -> Relays
// Receives hex relay commands from LoRa and controls relays

#include <Arduino.h>
#include <Wire.h>
#include "lora_config_esp.h"
#include "LoRaModule_esp.h"

#define RX_PIN 8  // GPIO44 D7 (RX on XIAO) - connects to LoRa TX
#define TX_PIN 7  // GPIO43 D6 (TX on XIAO) - connects to LoRa RX

// I2C address for this ESP when acting as a slave (change if needed)
#define I2C_SLAVE_ADDR 0x09
#define OPEN_ALL_VALVES 0xFE00  // binary 1111111000000000 (bits 15..9 = 1)
#define RELAY1 1
#define RELAY2 2
#define RELAY3 3
#define RELAY4 4
#define RELAY5 44
#define RELAY6 43

// Array of relay pins - using GPIO numbers that correspond to D0-D5 on XIAO
const uint8_t relayPins[6] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6};

// Create LoRa module instance
LoRaModule lora(RX_PIN, TX_PIN, LORA_RECEIVER_ADDRESS);

// I2C state (updated by master writes)
volatile uint16_t lastI2CValue = RELAY_MSB_BIT; // MSB set by default so master reads are valid

// Function Prototypes
void setRelays(uint16_t state);
bool parseHexToUint16(const String &hex, uint16_t &out); // parse hex string to uint16_t
void receiveEvent(int howMany);                          // I2C receive handler (Wire.onReceive)
void requestEvent();                                     // I2C request handler (Wire.onRequest)

void setup() {
  Serial.begin(115200);           // USB debug serial
  delay(2000);  // Wait for serial to initialize
  
  Serial.println("STARTING RECEIVER...");
  
  // Start I2C (Wire) as slave
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.print("Wire (I2C) initialized as SLAVE @ 0x");
  Serial.println(I2C_SLAVE_ADDR, HEX);

  // ensure local state has MSB validation bit set so masters reading this device see a valid state
  lastI2CValue = RELAY_MSB_BIT;
  
  // Initialize relay pins as outputs
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);  // Start with no power (solenoids closed)
  }
  
  // Initialize and configure LoRa module
  if (lora.begin()) {
    lora.configure(LORA_RECEIVER_ADDRESS, LORA_BAND, LORA_NETWORK_ID);
    Serial.println("✓ LoRa configured successfully");
  } else {
    Serial.println("✗ Failed to initialize LoRa!");
  }
}

void loop() {
  String hexData;
  
  // Use LoRa module to receive data
  if (lora.receiveData(hexData)) {
    uint16_t receivedBytes;
    if (!parseHexToUint16(hexData, receivedBytes)) {
      delay(50);
      return;
    }

    lastI2CValue = receivedBytes;

    bool validCommand = (receivedBytes & RELAY_MSB_BIT);
    if (validCommand) {
      setRelays(receivedBytes);
    }
  }
  
  delay(50);
}

// write all relays at once from a 6-bit value
// Relays are mapped to bits 9..14 (relay1 = bit14 ... relay6 = bit9)
// Bit==1 indicates valve OPEN. Hardware expects active-low signal (LOW energizes relay/solenoid).
void setRelays(uint16_t state) {
    for (uint8_t i = 0; i < 6; ++i) {
        // map relay i (0..5) to bit index 14..9 so bit14 opens relay1
        bool on = state & (1u << (RELAY_BIT_START + 5 - i));  // check bits 14-9 for relays
        // 1 => OPEN => drive pin LOW (active-low)
        digitalWrite(relayPins[i], on ? LOW : HIGH);
    }
}

// Parse a hex string (e.g. "1A3F") into a uint16_t. Returns true on success.
bool parseHexToUint16(const String &hex, uint16_t &out) {
  char *endptr = nullptr;
  long val = strtol(hex.c_str(), &endptr, 16);
  if (endptr == hex.c_str() || val < 0 || val > 0xFFFF) {
    return false;
  }
  out = (uint16_t)val;
  return true;
}

// I2C receive handler — called when an I2C master writes to this device
void receiveEvent(int howMany) {
  // Expect exactly 2 bytes; anything else is unexpected and will be drained.
  if (howMany != 2) {
    while (Wire.available()) {
      Wire.read();
    }
    return;               // ignore the entire write
  }

  // Normal 2-byte path
  uint8_t high = Wire.read();
  uint8_t low  = Wire.read();
  uint16_t value = (uint16_t(high) << 8) | low;

  lastI2CValue = value;
  if (value & RELAY_MSB_BIT) {
    setRelays(value);
  }
}

// I2C request handler — called when an I2C master requests data from this slave
void requestEvent() {
  uint8_t high = (uint8_t)(lastI2CValue >> 8);
  uint8_t low = (uint8_t)(lastI2CValue & 0xFF);
  Wire.write(high);
  Wire.write(low);
}
