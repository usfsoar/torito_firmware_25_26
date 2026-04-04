# LoraSend

**Purpose:** Drain **`lora_buffer`**, **serialize** a compact binary **header** (not full `payload[]`), send via **`LoraModule::send_data_hexstr`**, with **offline probe** and **backoff** on failure.

**Primary files:** `lorasend.h`, `lorasend.cpp`

**Used by:** Teensy (`env:teensy`) — `lora_sender.send_next()` in `main.cpp`.

**Documentation:** [LoRa protocols](../../docs/protocols/lora.md) · [Ring buffers](../../docs/reference/ring-buffers-and-dispatcher.md) · [Library index](../../docs/libraries/index.md)
