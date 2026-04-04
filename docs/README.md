# Torito firmware documentation

This folder is the main documentation tree for **torito_firmware_25_26**: a [PlatformIO](https://platformio.org/) project with multiple board targets (Teensy 4.1 DAQ, Seeed XIAO ESP32-S3 variants).

## Quick links

| Topic | Document |
|--------|-----------|
| Project intro and repo map | [Root README](../README.md) |
| First-time setup | [Getting started](getting-started.md) |
| How the system fits together | [Architecture — system overview](architecture/system-overview.md) |
| Build, environments, flashing | [Build — PlatformIO](build/platformio.md) |
| Teensy DAQ firmware | [Firmware — Teensy DAQ](firmware/teensy-daq.md) |
| GSC ESP — dual LoRa bridge | [Firmware — GSC ESP](firmware/gscesp-lora-bridge.md) |
| Torito ESP — relay + LoRa | [Firmware — Torito ESP](firmware/tsesp-relay-controller.md) |
| `SampleFrame` and buffers | [Reference — sample frame](reference/sample-frame.md) |
| LoRa AT usage and addresses | [Protocols — LoRa](protocols/lora.md) |
| Relay command bitmap | [Protocols — relay bitmap](protocols/relay-bitmap.md) |
| I2C solenoid / relay word | [Protocols — I2C solenoid](protocols/i2c-solenoid.md) |
| Configuration headers | [Reference — configuration](reference/configuration-headers.md) |
| Library modules (`lib/`) | [Libraries index](libraries/index.md) |
| USB serial behavior | [Operations — USB serial](operations/usb-serial.md) |
| Known gaps / next steps | [Roadmap](roadmap.md) |

## Documentation map

```
docs/
├── README.md                 ← You are here
├── getting-started.md
├── architecture/
│   └── system-overview.md
├── build/
│   ├── platformio.md
│   └── environments.md
├── firmware/
│   ├── teensy-daq.md
│   ├── gscesp-lora-bridge.md
│   └── tsesp-relay-controller.md
├── protocols/
│   ├── lora.md
│   ├── relay-bitmap.md
│   └── i2c-solenoid.md
├── reference/
│   ├── sample-frame.md
│   ├── configuration-headers.md
│   └── ring-buffers-and-dispatcher.md
├── libraries/
│   └── index.md
├── operations/
│   └── usb-serial.md
└── roadmap.md
```

## Conventions

- **Paths** in this documentation are relative to the repository root unless stated otherwise.
- **Firmware names** match PlatformIO environments in [`platformio.ini`](../platformio.ini): `teensy`, `gscesp`, `tsesp`.
