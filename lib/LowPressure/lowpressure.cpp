// pressure.cpp
#include "lowpressure.h"

// Define static constants
const float LowPressureSensor::V_MIN = 0.5f;
const float LowPressureSensor::V_MAX = 4.5f;
const float LowPressureSensor::PSI_MAX = 2000.0f;

bool LowPressureSensor::init() {
    if (!ads.begin(ADS1115_I2C_ADDR)) {
        return false;
    }
    ads.setGain(GAIN_TWOTHIRDS);
    return true;
}

bool LowPressureSensor::read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    raw_adc = ads.readADC_SingleEnded(sensor.adc_channel);
    return calibrate_from_raw(sensor, raw_adc, data);
}

bool LowPressureSensor::calibrate_from_raw(const SensorDesc &, int16_t raw_adc, int32_t &data) {
    const float voltage = static_cast<float>(raw_adc) * 0.1875f / 1000.0f;
    float psi = (voltage - V_MIN) * (PSI_MAX / (V_MAX - V_MIN));
    if (psi < 0.0f) psi = 0.0f;
    if (psi > PSI_MAX) psi = PSI_MAX;
    
    data = (int32_t)(psi * 100.0f);
    
    // Debug: check if reading makes sense
    if (raw_adc == -1 || raw_adc == 0x7FFF || raw_adc == 0 || voltage < 0.1f) {
        return false;  // Invalid reading
    }
    
    return true;
}