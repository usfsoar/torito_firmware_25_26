# Firmware: Torito ESP — relay + LoRa (`env: tsesp`)

[← Back to documentation index](../README.md)

## Role

This firmware runs on a **Seeed XIAO ESP32-S3** and acts as:

1. **LoRa receiver** — reads hex payloads from a LoRa module on **`Serial1`** with pins **`RX_PIN` / `TX_PIN`** (see [`src/tsesp/main.cpp`](../../src/tsesp/main.cpp)).  
2. **Relay driver** — maps a **16-bit command word** to **six** GPIOs (solenoids/valves), **active low** (LOW = energized).  
3. **I2C slave** at **`0x09`** — accepts **2-byte** writes and exposes the last valid state on read (for a Teensy or other master).

It uses the ESP-specific driver in [`lib/ESPLoRaModule`](../../lib/ESPLoRaModule) and [`lora_config_esp.h`](../../lib/ESPLoRaModule/lora_config_esp.h) (not the Teensy `lib/LoRaModule` + `lora_config.h`).

## Relay mapping

- **`setRelays(uint16_t state)`** uses bits **14 down to 9** for relays 1–6 (see comments in source).  
- Commands are accepted only if **`RELAY_MSB_BIT` (`0x8000`)** is set — same convention as [`lora_config.h`](../../lib/LoRaModule/lora_config.h) / ESP config headers.  
- **OPEN** = bit **1** → GPIO driven **LOW**; **closed** → **HIGH**.

See [Relay bitmap](../protocols/relay-bitmap.md).

## LoRa path

- `LoRaModule::receiveData(hexData)` parses **`+RCV=...`** lines and returns the **hex payload string**.  
- `parseHexToUint16()` converts to `uint16_t` and updates **`lastI2CValue`**.  
- If valid (`MSB` set), **`setRelays()`** runs.

## I2C slave

- **`Wire.begin(I2C_SLAVE_ADDR)`** with **`onReceive` / `onRequest`**.  
- **Write:** expects **exactly 2 bytes**; other lengths are drained and ignored.  
- **Request:** returns **high then low byte** of `lastI2CValue`.

This aligns with **`SolenoidReceive`** on the Teensy ([`lib/SolenoidReceive`](../../lib/SolenoidReceive)), which performs a **2-byte** `requestFrom` to the same address when the mux selects that downstream bus.

## Configuration alignment

[`lora_config_esp.h`](../../lib/ESPLoRaModule/lora_config_esp.h) defines:

- `LORA_BAND` **928000000** (vs Teensy **`lora_config.h`** **915000000** in-repo)  
- `LORA_RECEIVER_ADDRESS` **2** (vs Teensy **`LORA_RECEIVER_ADDRESS` 4** in `lora_config.h`)

For a working over-the-air link, **band, network ID, spreading-factor parameters, and module addresses** must be made consistent across **sender and receiver** in hardware configuration. Treat the headers as the **single source of truth** once you standardize them.

## See also

- [System overview](../architecture/system-overview.md)  
- [LoRa protocols](../protocols/lora.md)  
- [Relay bitmap](../protocols/relay-bitmap.md)  
