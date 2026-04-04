# I2C: solenoid state (16-bit word)

[← Back to documentation index](../README.md)

## Teensy (master read)

[`SolenoidReceive`](../../lib/SolenoidReceive/solenoidreceive.cpp) performs **`Wire.requestFrom(slave, 2)`** and combines high/low bytes **MSB first** into `uint16_t`. The DAQ selects the mux channel first when [`SOLENOID_MUX_CHANNEL`](../../lib/DataTypes/hwconfig.h) is not `0xFF` ([`daqloop.cpp`](../../lib/DAQLoop/daqloop.cpp)).

- **Address:** `SOLENOID_I2C_ADDR` (default **`0x09`** in `hwconfig.h`).

## Torito ESP (slave)

[`src/tsesp/main.cpp`](../../src/tsesp/main.cpp) registers **`Wire`** as a slave at **`I2C_SLAVE_ADDR`** (`0x09`):

- **On receive (master write):** expects **2 bytes**; updates `lastI2CValue` and may call **`setRelays()`** if **`RELAY_MSB_BIT`** is set.  
- **On request (master read):** returns the same **high / low** byte pair.

## End-to-end

Teensy **reads** the state the ESP **last applied** (LoRa or I2C write). Keep **bit semantics** aligned with [relay bitmap](relay-bitmap.md) and LoRa payloads.
