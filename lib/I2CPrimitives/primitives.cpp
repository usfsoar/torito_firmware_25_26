#include "primitives.h"

bool i2c_write(uint8_t i2c_address, const uint8_t *data, size_t length, bool send_stop) {
    Wire.beginTransmission(i2c_address);
    Wire.write(data, length);
    uint8_t result = Wire.endTransmission(send_stop);
    return (result == 0);
}

bool i2c_read(uint8_t i2c_address, uint8_t *buffer, size_t length) {
    size_t got = Wire.requestFrom((int)i2c_address, (int)length, (int)true);
    if (got != length) return false;
    for (size_t i = 0; i < length; i++) {
        buffer[i] = Wire.read();
    }
    return true;
}

bool i2c_write_read(uint8_t i2c_address, uint8_t reg, uint8_t *buffer, size_t length) {
    if (!i2c_write(i2c_address, &reg, 1, false)) {
        return false;
    }
    return i2c_read(i2c_address, buffer, length);
}