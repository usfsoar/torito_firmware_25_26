# LoRa: AT interface and telemetry

[← Back to documentation index](../README.md)

## Module behavior

The project uses **AT-command** LoRa modules (UART, typically **115200** baud). Teensy code wraps this in [`LoraModule`](../../lib/LoRaModule/loramodule.cpp); ESP relay code uses [`LoRaModule_esp`](../../lib/ESPLoRaModule/LoRaModule_esp.cpp).

Common commands:

| Command | Purpose |
|---------|---------|
| `AT` | Module probe |
| `AT+ADDRESS=<n>` | Module address on the network |
| `AT+BAND=<hz>` | Carrier frequency |
| `AT+NETWORKID=<n>` | Logical network ID |
| `AT+PARAMETER=SF,BW,CR,preamble` | Radio parameters (see below) |
| `AT+SEND=<dest>,<len>,<payload>` | Send; payload often **hex string** |

Default parameter string in firmware: **`11,9,4,24`** (matches comments in Teensy `configure()`).

## Teensy configuration (`lib/LoRaModule/lora_config.h`)

| Define | Typical in-repo value | Notes |
|--------|------------------------|--------|
| `LORA_BAND` | 915000000 | US band example |
| `LORA_NETWORK_ID` | 18 | Must match peers |
| `LORA_SENDER_ADDRESS` | 3 | Teensy node |
| `LORA_RECEIVER_ADDRESS` | 4 | Intended RX node address for `AT+SEND` |

## ESP relay configuration (`lib/ESPLoRaModule/lora_config_esp.h`)

| Define | In-repo value | Notes |
|--------|-----------------|--------|
| `LORA_BAND` | 928000000 | Differs from Teensy header — align for OTA |
| `LORA_RECEIVER_ADDRESS` | 2 | Differs from Teensy `LORA_RECEIVER_ADDRESS` — align with actual module role |

## Inbound message format

Modules emit lines like:

`+RCV=<sender>,<length>,<hexdata>,<RSSI>,<SNR>`

Parsers in **`LoraModule::receive_data_hexstr`** (Teensy) and **`LoRaModule::receiveData`** (ESP) extract the **hex payload** field.

## Teensy outbound telemetry (`LoraSend`)

[`LoraSend`](../../lib/LoraSend/lorasend.cpp) **does not** send the full `SampleFrame` over the air. It serializes a **fixed header** (up to **20 bytes**):

- `timestamp_us` (4), `seq` (4), `valid_mask` (1), `status_bits` (1), `solenoid_state` (2), `raw_adc[SENSOR_COUNT]` (2 bytes × count)

**Processed pressure values** in `payload[]` are **not** included in this serialization (verify against your ground decoder).

Sending uses **`send_data_hexstr`** with raw bytes → module encodes as hex in `AT+SEND`. The implementation accounts for **module quirks** (hex digit count vs byte count) — see comments in `loramodule.cpp`.

## Offline / backoff

`LoraSend` probes the module with **`ping()`** if `is_online()` is false and applies **backoff** (e.g. 5 s) after failed sends to avoid blocking the main loop.

## Related

- [Relay bitmap](relay-bitmap.md)  
- [I2C solenoid word](i2c-solenoid.md)  
- [Teensy DAQ firmware](../firmware/teensy-daq.md)  
- [GSC ESP bridge](../firmware/gscesp-lora-bridge.md)  
