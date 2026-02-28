#include "temperature.h"

const float TemperatureSensor::TEMP_MIN = 0.0;
const float TemperatureSensor::TEMP_MAX = 5.0;
const float TemperatureSensor::CELSIUS_MAX = 100.0;

bool TemperatureSensor::init() {
    if (!ads.begin()) {
        return false;
    }
    return true;
}

bool TemperatureSensor::read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    raw_adc = ads.readADC_SingleEnded(sensor.adc_channel);
    float voltage = raw_adc * 0.1875 / 1000; // Convert to voltage
    data = (voltage - TEMP_MIN) * (CELSIUS_MAX / (TEMP_MAX - TEMP_MIN));
    return true;
}