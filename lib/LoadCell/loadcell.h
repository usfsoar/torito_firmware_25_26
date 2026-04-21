#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <hwconfig.h>
#include <SensorDesc.h>

class LoadCell {
private:
    static Adafruit_ADS1115 ads;
    static long offset_counts;
    static constexpr uint8_t kDefaultTareSamples = 5;
    static constexpr uint8_t kDefaultAdcChannel = 0;

public:
    bool init();
    bool read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc);
    bool calibrate_from_raw(const SensorDesc &sensor, int16_t raw_adc, int32_t &data);
    bool set_zero(uint8_t samples = 5);
    long get_offset_counts() const;

};

#endif