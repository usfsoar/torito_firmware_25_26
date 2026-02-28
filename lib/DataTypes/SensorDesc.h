#ifndef SENSOR_DESC_H
#define SENSOR_DESC_H

#include <stdint.h>

struct SensorDesc {
    uint8_t id;
    enum type {
        TEMPERATURE,
        LOWPRESSURE,
        HIGHPRESSURE
    } sensorType;
    uint8_t bus_id;
    uint8_t mux_channel;
    uint8_t i2c_address;
    uint16_t adc_channel;
    uint16_t period_ticks;
};

#endif // SENSOR_DESC_H