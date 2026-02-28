#ifndef SAMPLEFRAME_H
#define SAMPLEFRAME_H

#include <sensorconfig.h>
#include <stdint.h>
#include <type_traits>

struct SampleFrame {
    uint32_t timestamp_us;
    uint32_t seq;
    uint8_t valid_mask;
    uint8_t status_bits;
    uint16_t solenoid_state;
    uint32_t payload[SENSOR_COUNT];
    uint16_t raw_adc[SENSOR_COUNT];
};

static_assert(std::is_trivially_copyable<SampleFrame>::value,
              "SampleFrame must be trivially copyable for raw/memcpy serialization");
static_assert(std::is_standard_layout<SampleFrame>::value,
              "SampleFrame must be standard-layout for stable memory layout");

#endif // SAMPLEFRAME_H