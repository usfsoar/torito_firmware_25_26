#ifndef MUXDRIVER_H
#define MUXDRIVER_H

#include <stdint.h>
#include <primitives.h>

#define MUX_ADDRESS 0x70

bool mux_init();
bool mux_select(uint8_t bus_id, uint8_t mux_channel);

#endif // MUXDRIVER_H