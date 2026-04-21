#ifndef SENSOR_DISPATCHER_H
#define SENSOR_DISPATCHER_H

#include <SensorDesc.h>
#include <lowpressure.h>
#include <highpressure.h>
#include <temperature.h>
#include <loadcell.h>
#include <cstdint>

bool sensor_dispatcher_init();
bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc);

#endif // SENSOR_DISPATCHER_H