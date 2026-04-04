# Relay command bitmap (16-bit)

[← Back to documentation index](../README.md)

## Validity bit

Both Teensy [`lora_config.h`](../../lib/LoRaModule/lora_config.h) and ESP [`lora_config_esp.h`](../../lib/ESPLoRaModule/lora_config_esp.h) define:

- **`RELAY_MSB_BIT`** = **`0x8000`** — bit **15** must be **1** for a command to be treated as **valid**.

If this bit is clear, relay updates from LoRa or I2C (depending on firmware path) should **not** change outputs.

## Relay bit layout (Torito ESP)

In [`src/tsesp/main.cpp`](../../src/tsesp/main.cpp), **`setRelays()`** maps **six** relays to bits **14 through 9**:

- **Relay 1** ↔ bit **14**  
- **Relay 2** ↔ bit **13**  
- …  
- **Relay 6** ↔ bit **9**

**Bit = 1** means **valve open**; the GPIO is driven **LOW** (active-low hardware).

## Special values

Source references:

- **`OPEN_ALL_VALVES`** = **`0xFE00`** — binary pattern with bits **15–9** set (documentation in `tsesp` main).  
- GSC ESP **repeat mode** toggles between **`8000`** and **`fe00`** for testing.

Always interpret together with **`RELAY_MSB_BIT`**: e.g. **`0x8000`** sets validity and may encode a specific solenoid pattern depending on bits 14–9.

## Consistency with DAQ

The Teensy records **`solenoid_state`** in each `SampleFrame` ([`SampleFrame.h`](../../lib/DataTypes/SampleFrame.h)) from **`SolenoidReceive`** — a **2-byte** value reflecting what the I2C slave last reported / commanded. Ground software should use the same bit semantics as the ESP relay firmware when decoding logs and live RF.

## See also

- [Torito ESP firmware](../firmware/tsesp-relay-controller.md)  
