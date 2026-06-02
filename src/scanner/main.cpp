#include <Arduino.h>
#include <Wire.h>
#include <hwconfig.h>
#include <muxdriver.h>

static const uint8_t kMuxChannels = 8;
static const uint8_t kKnownProbeAddresses[] = {
    ADS1015_I2C_ADDR,
    SOLENOID_I2C_ADDR,
};

static bool i2c_probe_address(uint8_t address) {
    Wire.beginTransmission(address);
    uint8_t result = Wire.endTransmission();
    return (result == 0);
}

static bool probe_mux() {
    Serial.print("Probing mux at 0x");
    Serial.println(MUX_ADDRESS, HEX);
    return i2c_probe_address(MUX_ADDRESS);
}

static bool scan_mux_channel(uint8_t channel, uint8_t &found_address) {
    if (!mux_select(0, channel)) {
        Serial.print("ERROR: mux_select(");
        Serial.print(channel);
        Serial.println(") failed");
        return false;
    }
    delay(5); // allow mux to settle

    const size_t known_count = sizeof(kKnownProbeAddresses) / sizeof(kKnownProbeAddresses[0]);
    for (size_t i = 0; i < known_count; ++i) {
        uint8_t address = kKnownProbeAddresses[i];
        if (i2c_probe_address(address)) {
            found_address = address;
            return true;
        }
    }

    for (uint8_t address = 0x03; address <= 0x77; ++address) {
        if (address == MUX_ADDRESS) {
            continue;
        }
        bool skipKnown = false;
        for (size_t i = 0; i < known_count; ++i) {
            if (address == kKnownProbeAddresses[i]) {
                skipKnown = true;
                break;
            }
        }
        if (skipKnown) {
            continue;
        }

        if (i2c_probe_address(address)) {
            found_address = address;
            return true;
        }
    }

    return false;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println("Teensy MUX scanner starting...");

    Wire.begin();

    if (!probe_mux()) {
        Serial.println("ERROR: MUX not found at 0x70");
        while (true) {
            delay(1000);
        }
    }

    while (!mux_init()) {
        Serial.println("ERROR: MUX init failed!");
        delay(1000);
    }

    uint8_t found_channel = 0xFF;
    uint8_t found_address = 0;

    for (uint8_t channel = 0; channel < kMuxChannels; ++channel) {
        Serial.print("Selecting mux channel ");
        Serial.print(channel);
        Serial.print(" on 0x");
        Serial.println(MUX_ADDRESS, HEX);

        if (scan_mux_channel(channel, found_address)) {
            found_channel = channel;
            break;
        }

        Serial.print("  no device found behind channel ");
        Serial.println(channel);
    }

    if (found_channel != 0xFF) {
        Serial.print("Device found on mux channel ");
        Serial.print(found_channel);
        Serial.print(" at I2C address 0x");
        Serial.println(found_address, HEX);
    } else {
        Serial.println("No I2C device found on any mux channel.");
    }
}

void loop() {
    delay(1000);
}
