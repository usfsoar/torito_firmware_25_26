# BufferDispatcher

**Purpose:** Move `SampleFrame` records from the DAQ ring buffer to the SD and LoRa ring buffers (fan-out).

**Primary files:** `dispatcherthread.h`, `dispatcherthread.cpp`

**Behavior:** Pops one frame from `daq_buffer` when available; pushes every frame to `sd_buffer`; pushes every **10th** frame (`seq % 10 == 0`) to `lora_buffer`. Returns `false` if a downstream `push` fails (overflow).

**Used by:** Teensy (`env:teensy`) — `dispatcher_thread_step()` from `main.cpp`.

**Documentation:** [Ring buffers & dispatcher](../../docs/reference/ring-buffers-and-dispatcher.md) · [Library index](../../docs/libraries/index.md)
