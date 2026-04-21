#ifndef DAQLOOP_H
#define DAQLOOP_H

#include <SampleFrame.h>
#include <ringbuffer.h>
#include <sensordispatcher.h>
#include <muxdriver.h>
#include <sensorconfig.h>
#include <cstdint>
#include <solenoidreceive.h>
#include <hwconfig.h>
#include <adcstate.h>

extern SolenoidReceive solenoid_receive;

// Status bit flags
#define MUX_ERR    (1 << 0)
#define I2C_ERR    (1 << 1)
#define OVERRUN    (1 << 2)

void daq_init();
void daq_step();

#endif // DAQLOOP_H
