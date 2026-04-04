# Torito firmware (`torito_firmware_25_26`)

Firmware for **Torito EECS**: a multi-target **[PlatformIO](https://platformio.org/)** project that runs **data acquisition** on a **Teensy 4.1**, streams telemetry over **LoRa**, logs to **SD card**, and drives **solenoid/relay** hardware from **Seeed XIAO ESP32-S3** boards.

## What is in this repository

| Area | Description |
|------|-------------|
| **Teensy 4.1** (`env:teensy`) | DAQ loop, TCA9548A mux, ADS1115 pressure, solenoid state over I2C, SD logging, LoRa send |
| **GSC ESP32-S3** (`env:gscesp`) | Dual UART bridge: two LoRa modules ↔ USB (AT commands, binary-framed RX to host) |
| **Torito ESP32-S3** (`env:tsesp`) | LoRa receive → 16-bit relay command; I2C slave at `0x09` for the same state word |
| **`lib/`** | Shared modules: DAQ, ring buffers, dispatcher, LoRa (Teensy vs ESP variants), SD writer, sensors |

Sources are split by **build target** under [`src/`](src) (only one `main` tree is built per environment — see [`platformio.ini`](platformio.ini)).

## Documentation (start here)

All structured documentation lives under **[`docs/`](docs/README.md)**:

| Document | Content |
|----------|---------|
| [**Documentation home**](docs/README.md) | Full index and folder map |
| [**System architecture**](docs/architecture/system-overview.md) | Block diagram and data flow |
| [**Build & PlatformIO**](docs/build/platformio.md) | Compile, upload, layout |
| [**Environments**](docs/build/environments.md) | `teensy` / `gscesp` / `tsesp` details |
| [**Teensy DAQ**](docs/firmware/teensy-daq.md) | Main flight/DAQ firmware |
| [**GSC ESP (LoRa bridge)**](docs/firmware/gscesp-lora-bridge.md) | Dual-radio USB bridge |
| [**Torito ESP (relays)**](docs/firmware/tsesp-relay-controller.md) | Relay + LoRa + I2C |
| [**LoRa protocols**](docs/protocols/lora.md) | AT commands, telemetry format |
| [**Relay bitmap**](docs/protocols/relay-bitmap.md) | 16-bit command encoding |
| [**I2C solenoid word**](docs/protocols/i2c-solenoid.md) | Teensy ↔ ESP 2-byte state |
| [**SampleFrame reference**](docs/reference/sample-frame.md) | Binary DAQ record |
| [**Configuration headers**](docs/reference/configuration-headers.md) | Where to change IDs, mux, LoRa |
| [**Ring buffers**](docs/reference/ring-buffers-and-dispatcher.md) | DAQ → SD / LoRa fan-out |
| [**Libraries index**](docs/libraries/index.md) | All `lib/` modules |
| [**Roadmap / follow-ups**](docs/roadmap.md) | Safety, testing, known gaps |
| [**Getting started**](docs/getting-started.md) | Prerequisites and first build |
| [**USB serial**](docs/operations/usb-serial.md) | Debug output and GSC host format |

## Quick build

```bash
pio run -e teensy
pio run -e gscesp
pio run -e tsesp
```

Set **`upload_port`** / **`monitor_port`** in [`platformio.ini`](platformio.ini) to match your USB serial devices.

## Project notes

- **Next steps and risks** (integrity, watchdog, timeouts, relay/DAQ stress tests): [`NEXTSTEPS.md`](NEXTSTEPS.md) — summarized in [`docs/roadmap.md`](docs/roadmap.md).
- **PlatformIO** placeholders: [`lib/README`](lib/README), [`test/README`](test/README), [`include/README`](include/README).

## Repository layout (abbreviated)

```
├── platformio.ini          # Environments and board settings
├── README.md               # This file
├── NEXTSTEPS.md            # Working notes
├── docs/                   # Full documentation tree
├── src/
│   ├── teensy/main.cpp     # Teensy DAQ entry
│   ├── gscesp/main.cpp     # GSC dual-LoRa bridge entry
│   └── tsesp/main.cpp      # Relay ESP entry
├── lib/                    # Firmware libraries (DAQ, LoRa, SD, …)
└── test/                   # Reserved for PlatformIO unit tests
```

---

*Torito — GSC + DAQ firmware, 2025–26.*
