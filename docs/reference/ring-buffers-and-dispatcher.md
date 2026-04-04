# Ring buffers and dispatcher

[← Back to documentation index](../README.md)

## Globals (`src/teensy/main.cpp`)

Three **`RingBuffer`** instances:

| Buffer | Producer | Consumer |
|--------|----------|----------|
| `daq_buffer` | `daq_step()` | `dispatcher_thread_step()` |
| `sd_buffer` | dispatcher | `SDWrite::data()` |
| `lora_buffer` | dispatcher | `LoraSend::send_next()` |

## `RingBuffer`

[`lib/RingBuffer`](../../lib/RingBuffer) — fixed length **`RING_BUFFER_SIZE`** (255) of **`SampleFrame`**. **`push`** on full increments **`overrun_count`** and returns false.

## Dispatcher

[`lib/BufferDispatcher/dispatcherthread.cpp`](../../lib/BufferDispatcher/dispatcherthread.cpp):

1. **Pop** one frame from **`daq_buffer`** (if any).  
2. **Push** to **`sd_buffer`** (every popped frame).  
3. If **`frame.seq % 10 == 0`**, also **push** to **`lora_buffer`**.

So **LoRa** receives **one tenth** of the DAQ rate **in the current implementation** (not every frame).

Return **`false`** if `sd_buffer` or `lora_buffer` **push** fails (overflow); Teensy main prints an overflow message.

## SD writer

[`SDWrite::data()`](../../lib/SDWrite/sdwrite.cpp) batches binary **`SampleFrame`** writes into a 4 KiB RAM buffer, flushes to **`data.bin`**, and uses a **session start marker** (`0xA5A5A5A5`) at init.

## Related

- [Sample frame](sample-frame.md)  
- [Teensy DAQ](../firmware/teensy-daq.md)  
