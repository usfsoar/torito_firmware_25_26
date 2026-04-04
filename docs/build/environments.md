# PlatformIO environments

[← Back to documentation index](../README.md) · [Build overview](platformio.md)

Definitions live in [`platformio.ini`](../../platformio.ini). Values below reflect the checked-in file; **COM ports are machine-specific** and must match your OS.

## Summary

| Environment | Platform | Board | Framework | Monitor baud |
|-------------|----------|--------|-----------|--------------|
| `teensy` | `teensy` | `teensy41` | Arduino | 115200 |
| `gscesp` | `espressif32` | `seeed_xiao_esp32s3` | Arduino | 115200 |
| `tsesp` | `espressif32` | `seeed_xiao_esp32s3` | Arduino | 115200 |

## `teensy`

- **Extra dependencies:** `adafruit/Adafruit ADS1X15`, `adafruit/Adafruit BusIO` (`lib_ldf_mode = deep+` helps dependency resolution).  
- **Upload/monitor ports:** example `COM8` in repo — change to your Teensy port.  
- **Entry point:** [`src/teensy/main.cpp`](../../src/teensy/main.cpp).

## `gscesp`

- **Upload/monitor ports:** example `COM6` — change as needed.  
- **Entry point:** [`src/gscesp/main.cpp`](../../src/gscesp/main.cpp).  
- Dual hardware UARTs for two LoRa modules (see [GSC ESP firmware](../firmware/gscesp-lora-bridge.md)).

## `tsesp`

- **Upload/monitor ports:** example `COM7` — change as needed.  
- **Entry point:** [`src/tsesp/main.cpp`](../../src/tsesp/main.cpp).  
- Uses [`lib/ESPLoRaModule`](../../lib/ESPLoRaModule) (ESP variant of LoRa driver), not the Teensy `LoraModule` under `lib/LoRaModule`.

## Port configuration

On Windows, Device Manager lists COM ports; on Linux, devices are often `/dev/ttyACM*` or `/dev/ttyUSB*`. Update `upload_port` and `monitor_port` in `platformio.ini` to match the board you are flashing.
