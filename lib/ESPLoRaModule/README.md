# ESPLoRaModule

**Purpose:** **ESP32** LoRa module driver: UART `Serial1`, AT commands, `+RCV` parsing. Pairs with **`lora_config_esp.h`** (band, addresses, network ID, relay bitmask constants).

**Primary files:** `LoRaModule_esp.h`, `LoRaModule_esp.cpp`, `lora_config_esp.h`

**Used by:** `env:tsesp` ([`src/tsesp/main.cpp`](../../src/tsesp/main.cpp)) — not the Teensy `lib/LoRaModule` stack.

**Documentation:** [Torito ESP firmware](../../docs/firmware/tsesp-relay-controller.md) · [LoRa protocols](../../docs/protocols/lora.md) · [Library index](../../docs/libraries/index.md)
