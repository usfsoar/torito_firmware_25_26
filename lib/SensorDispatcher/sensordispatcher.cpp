#include "sensordispatcher.h"

// Sesnor class instances
LowPressureSensor g_low_pressure_sensor;
HighPressureSensor g_high_pressure_sensor;
TemperatureSensor g_temperature_sensor;
LoadCell g_load_cell;

bool sensor_dispatcher_init() {
    if (!g_low_pressure_sensor.init()) {
        Serial.println("ERROR: ADS1115 (low pressure sensor) init failed!");
        return false;
    }
    
    if (!g_high_pressure_sensor.init()) {
        Serial.println("ERROR: ADS1115 (high pressure sensor) init failed!");
        return false;
    }

    if (!g_temperature_sensor.init()) {
        Serial.println("ERROR: Temperature sensor init failed!");
        return false;
    }

    if (!g_load_cell.init()) {
        Serial.println("ERROR: Load cell init failed!");
        return false;
    }
    
    return true;
}

bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    switch (sensor.sensorType) {
        case SensorDesc::type::LOWPRESSURE:
            return g_low_pressure_sensor.read(sensor, data, raw_adc);
        case SensorDesc::type::HIGHPRESSURE:
            return g_high_pressure_sensor.read(sensor, data, raw_adc);
        case SensorDesc::type::TEMPERATURE:
            return g_temperature_sensor.read(sensor, data, raw_adc);
        case SensorDesc::type::LOADCELL:
            return g_load_cell.read(sensor, data, raw_adc);
        default:
            return false;
    }
}

bool sensor_calculate_dispatch(const SensorDesc &sensor, int16_t raw_adc, int32_t &data) {
    switch (sensor.sensorType) {
        case SensorDesc::type::LOWPRESSURE:
            return g_low_pressure_sensor.calibrate_from_raw(sensor, raw_adc, data);
        case SensorDesc::type::HIGHPRESSURE:
            return g_high_pressure_sensor.calibrate_from_raw(sensor, raw_adc, data);
        case SensorDesc::type::TEMPERATURE:
            return g_temperature_sensor.calibrate_from_raw(sensor, raw_adc, data);
        case SensorDesc::type::LOADCELL:
            return g_load_cell.calibrate_from_raw(sensor, raw_adc, data);

        default:
            return false;
    }
}