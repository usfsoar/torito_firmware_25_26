#include "muxdriver.h"

bool mux_init() {
    // Initialize TCA9548A: disable all channels as known state
    uint8_t disable_all = 0x00;
    return i2c_write(MUX_ADDRESS, &disable_all, 1);
}

bool mux_select(uint8_t bus_id, uint8_t channel) {
    // bus_id parameter reserved for future multi-bus support
    // For now, always use default MUX_ADDRESS
    uint8_t mask = (1 << channel);
    return i2c_write(MUX_ADDRESS, &mask, 1);
}