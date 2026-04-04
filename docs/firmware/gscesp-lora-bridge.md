# Firmware: GSC ESP — dual LoRa bridge (`env: gscesp`)

[← Back to documentation index](../README.md)

## Role

This target runs on a **Seeed XIAO ESP32-S3** and implements a **ground / test bridge** between **USB serial** and **two LoRa radio modules** on separate hardware UARTs:

- **RXLORA** (`HardwareSerial` instance 1): pins documented in source as RX/TX for “LoRa 1” — receives traffic.  
- **TXLORA** (`HardwareSerial` instance 2): pins for “LoRa 2” — sends AT commands including `AT+SEND`.

**Entry point:** [`src/gscesp/main.cpp`](../../src/gscesp/main.cpp)

## Boot behavior

- USB `Serial` at **115200**.  
- Initializes both UARTs at **115200 8N1**.  
- Sends `AT` to each module, then configures **different** `AT+ADDRESS` values per UART, **bands** (915 MHz vs 928 MHz in the checked-in sketch), shared **network ID**, and **`AT+PARAMETER=11,9,4,24`**.

> **Note:** The Teensy stack uses a single band and matching network parameters in [`lora_config.h`](../../lib/LoRaModule/lora_config.h). Any **bridge** between GSC ESP and flight hardware must ensure **compatible** RF settings and **addressing** for the link under test.

## Receive path (LoRa → USB)

When a line containing **`+RCV=`** is assembled from **RXLORA**:

1. Payload field is parsed as **hex digits** and converted to **raw bytes**.  
2. USB sends: **`0xAA` `0x55`** followed by the **raw payload bytes** (`Serial.write`).

This gives host software a **binary framing** hook (`0xAA 0x55`) ahead of the packet body.

## Transmit path (USB → LoRa)

`checkUserInput()` reads a line from USB:

- Format: **`<command>,<address>`**  
  - Example: send hex payload `8000` to module address `4`: `8000,4`  
- Optional suffix **`:repeat`** on the command enables **reporting_lock**: once per second the firmware alternates payload between **`8000`** and **`fe00`** (test pattern), ignoring the user payload for content (address preserved).

`send_command()` builds **`AT+SEND=<addr>,<len>,<data>`** and sends it on **TXLORA** with a short AT response wait.

## Commented / legacy code

The file contains **commented** `Wire` slave handlers and alternate `loop` bodies — they are not active in the current build but document earlier I2C-to-host experiments.

## See also

- [LoRa protocols](../protocols/lora.md)  
- [Build environments](../build/environments.md)  
