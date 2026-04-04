# SolenoidReceive

**Purpose:** **I2C master** read of **16-bit** solenoid/relay state from the slave (default address from [`hwconfig.h`](../DataTypes/hwconfig.h)). Caches the last good value for use if mux selection fails.

**Primary files:** `solenoidreceive.h`, `solenoidreceive.cpp`

**API:** `init(i2c_addr)`, `read(out_state)`, `get_cached_state()`

**Note:** Does **not** select the mux — DAQ owns mux selection before `read()`.

**Used by:** Teensy (`env:teensy`) — paired with Torito ESP I2C slave in [`src/tsesp/main.cpp`](../../src/tsesp/main.cpp).

**Documentation:** [I2C solenoid protocol](../../docs/protocols/i2c-solenoid.md) · [Library index](../../docs/libraries/index.md)
