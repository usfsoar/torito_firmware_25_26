#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <hwconfig.h>
#include <SensorDesc.h>

class LoadCell {
public:
    bool init();
    bool read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc);
    bool set_zero(uint8_t samples = 5);
    long get_offset_counts() const;

};

#endif