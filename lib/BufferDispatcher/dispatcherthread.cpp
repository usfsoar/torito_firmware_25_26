#include <dispatcherthread.h>
// External buffers (declared in main)
extern RingBuffer daq_buffer;
extern RingBuffer sd_buffer;
extern RingBuffer lora_buffer;

bool dispatcher_thread_step() {
    // Example implementation
    SampleFrame frame;

    if (daq_buffer.pop(&frame)) {
        // Process frame and push to other buffers
        if (!sd_buffer.push(&frame)) {
            // Handle SD buffer overflow
            return false;
        }
        if (frame.seq % 10 == 0) { // Pushes every 10th frame
            if (!lora_buffer.push(&frame)) {
                // Handle LoRa buffer overflow
                return false;
            }
        }
    }
    return true;
}