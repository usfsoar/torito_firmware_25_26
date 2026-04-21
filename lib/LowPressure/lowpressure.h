// lowpressure.h
#ifndef LOWPRESSURE_H
#define LOWPRESSURE_H

#include <stdint.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <SensorDesc.h>
#include <hwconfig.h>


class LowPressureSensor {
private:
    static const float V_MIN;
    static const float V_MAX;
    static const float PSI_MAX;
    Adafruit_ADS1115 ads;

public:
    bool init();
    bool read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc);
    bool calibrate_from_raw(const SensorDesc &sensor, int16_t raw_adc, int32_t &data);
};

#endif // LOWPRESSURE_H