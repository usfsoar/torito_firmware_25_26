#ifndef HWCONFIG_H
#define HWCONFIG_H

// Teensy 4.1 uses default I2C Wire pins (SDA=18, SCL=19)
// Wire.begin() automatically uses these pins

// I2C address for the external solenoid-status MCU (7-bit)
// Change this if the slave device address is different on your bus.
#define SOLENOID_I2C_ADDR 0x09
#define ADS1115_I2C_ADDR 0x48

// HX711 load cell amplifier pins and gain
#define HX711_DOUT_PIN 2 // placeholder for HX711 data pin
#define HX711_SCK_PIN 3  // placeholder for HX711 clock pin 
#define HX711_GAIN 128

// If the solenoid MCU is connected behind a PCA9548A mux, set the channel
// (0..7). Set to 0xFF if attached to the main bus.
#define SOLENOID_MUX_CHANNEL 7
#define ADS_MUX_CHANNEL 4
#define NO_MUX 0xFF

#endif // HWCONFIG_H
