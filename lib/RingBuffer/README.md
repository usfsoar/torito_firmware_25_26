# RingBuffer

**Purpose:** **Fixed-size FIFO** of `SampleFrame` structs for DAQ → dispatcher → SD / LoRa pipelines.

**Primary files:** `ringbuffer.h`, `ringbuffer.cpp`

**Constants:** `RING_BUFFER_SIZE` (default 255). Tracks **overrun_count** when `push` fails while full.

**Used by:** Teensy `main.cpp` (`daq_buffer`, `sd_buffer`, `lora_buffer`).

**Documentation:** [Ring buffers & dispatcher](../../docs/reference/ring-buffers-and-dispatcher.md) · [Library index](../../docs/libraries/index.md)
