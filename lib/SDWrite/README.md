# SDWrite

**Purpose:** Drain **`sd_buffer`** into a **RAM block buffer**, then append **binary `SampleFrame`** blobs to an SD file (default **`data.bin`** on Teensy built-in SD). Writes a **session start marker** on init.

**Primary files:** `sdwrite.h`, `sdwrite.cpp`

**API:** `init(buffer, filename)`, `data()` (call from main loop), `force_flush()`

**Used by:** Teensy (`env:teensy`) only.

**Documentation:** [Ring buffers](../../docs/reference/ring-buffers-and-dispatcher.md) · [Teensy DAQ](../../docs/firmware/teensy-daq.md) · [Library index](../../docs/libraries/index.md)
