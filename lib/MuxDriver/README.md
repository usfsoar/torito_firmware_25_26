# MuxDriver

**Purpose:** Control the **TCA9548A** I2C multiplexer (**address `0x70`**): init (all channels off) and select one channel.

**Primary files:** `muxdriver.h`, `muxdriver.cpp`

**API:** `mux_init()`, `mux_select(bus_id, mux_channel)` — `bus_id` reserved for future multi-bus use.

**Used by:** Teensy DAQ and sensor init paths.

**Documentation:** [Configuration headers](../../docs/reference/configuration-headers.md) · [Library index](../../docs/libraries/index.md)
