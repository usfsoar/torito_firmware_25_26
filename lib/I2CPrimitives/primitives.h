#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <Wire.h>
#include <stdint.h>

bool i2c_write(uint8_t i2c_address, const uint8_t *data, size_t length, bool send_stop = true);
bool i2c_read(uint8_t i2c_address, uint8_t *buffer, size_t length);
bool i2c_write_read(uint8_t i2c_address, uint8_t reg, uint8_t *buffer, size_t length);

#endif // PRIMITIVES_H