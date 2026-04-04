# Getting started

[← Back to documentation index](README.md)

## 1. Install tools

- **PlatformIO** (VS Code extension recommended) or **PIO Core** CLI  
- USB drivers for **Teensy 4.1** and **ESP32-S3** (Seeed XIAO)

## 2. Open the project

Open the repository root (the folder that contains `platformio.ini`) as the PlatformIO project root.

## 3. Pick an environment

| Goal | Environment | Main doc |
|------|-------------|----------|
| Flight / lab DAQ on Teensy | `teensy` | [Teensy DAQ](firmware/teensy-daq.md) |
| Ground LoRa ↔ USB bridge | `gscesp` | [GSC ESP](firmware/gscesp-lora-bridge.md) |
| Relay + LoRa receiver ESP | `tsesp` | [Torito ESP](firmware/tsesp-relay-controller.md) |

## 4. Set serial ports

Edit [`platformio.ini`](../platformio.ini): set **`upload_port`** and **`monitor_port`** for your OS (e.g. Windows `COM#`, Linux `/dev/ttyACM0`). Details: [Environments](build/environments.md).

## 5. Build and upload

```bash
pio run -e teensy -t upload
```

Replace `teensy` with `gscesp` or `tsesp` as needed.

## 6. Monitor

Default baud rate in this project is **115200** for USB serial on all three targets.

## 7. Read next

- [System overview](architecture/system-overview.md) — how pieces connect  
- [Configuration headers](reference/configuration-headers.md) — addresses and LoRa IDs  
- [Roadmap](roadmap.md) — open engineering items  
