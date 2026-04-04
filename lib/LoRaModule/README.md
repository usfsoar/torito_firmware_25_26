# LoRaModule (Teensy)

**Purpose:** **Teensy** LoRa module over **AT commands** on **`Serial5`**, plus **`lora_config.h`** (band, network ID, node addresses, `AT+PARAMETER` defaults).

**Primary files:** `loramodule.h`, `loramodule.cpp`, `lora_config.h`

**API highlights:** `begin()`, `configure()`, `send_data_hexstr()`, `receive_data_hexstr()`, `ping()`, `is_online()`

**Used by:** `env:teensy` with [`LoraSend`](../LoraSend/README.md).

**Documentation:** [LoRa protocols](../../docs/protocols/lora.md) · [Teensy DAQ](../../docs/firmware/teensy-daq.md) · [Library index](../../docs/libraries/index.md)
