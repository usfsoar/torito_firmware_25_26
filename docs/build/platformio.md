# Build with PlatformIO

[← Back to documentation index](../README.md)

## Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- USB drivers appropriate for **Teensy 4.1** and **Seeed XIAO ESP32-S3**
- For Teensy: Adafruit libraries are pulled automatically (see [`platformio.ini`](../../platformio.ini) `lib_deps` under `env:teensy`)

## Common commands

From the repository root:

```bash
pio run -e teensy
pio run -e gscesp
pio run -e tsesp
```

Upload (after selecting the correct `upload_port` in `platformio.ini` for your machine):

```bash
pio run -e teensy -t upload
pio run -e gscesp -t upload
pio run -e tsesp -t upload
```

Serial monitor (baud **115200** for all environments in this project):

```bash
pio device monitor -b 115200
```

## Project layout

| Path | Role |
|------|------|
| [`platformio.ini`](../../platformio.ini) | Environments, board, `build_src_filter`, ports |
| [`src/teensy/`](../../src/teensy) | Teensy-only `main.cpp` |
| [`src/gscesp/`](../../src/gscesp) | GSC ESP-only `main.cpp` |
| [`src/tsesp/`](../../src/tsesp) | Torito relay ESP-only `main.cpp` |
| [`lib/`](../../lib) | Project libraries (DAQ, LoRa, SD, etc.) |

## Source filtering

Each environment compiles **only one** `main.cpp` tree via `build_src_filter`, for example:

- `+<teensy/*>` for Teensy  
- `+<gscesp/*>` / `+<tsesp/*>` for the ESP targets  

So shared code lives under `lib/`; entry points are isolated per target.

## Next steps

- [Environments and ports](environments.md)  
- [Teensy DAQ](../firmware/teensy-daq.md)  
