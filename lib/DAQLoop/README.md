# DAQLoop

**Purpose:** One DAQ tick: build a `SampleFrame` — mux select, read all scheduled sensors, read solenoid state, push to `daq_buffer`.

**Primary files:** `daqloop.h`, `daqloop.cpp`

**API:**

| Function | Role |
|----------|------|
| `daq_init()` | Reset sequence and tick counters |
| `daq_step()` | Acquire one frame; sets `status_bits` (`MUX_ERR`, `I2C_ERR`, `OVERRUN` on ring push failure) |

**Used by:** Teensy (`env:teensy`) only.

**Documentation:** [Teensy DAQ](../../docs/firmware/teensy-daq.md) · [Sample frame](../../docs/reference/sample-frame.md) · [Ring buffers](../../docs/reference/ring-buffers-and-dispatcher.md) · [Library index](../../docs/libraries/index.md)
