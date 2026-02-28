#ifndef DISPATCHERTHREAD_H
#define DISPATCHERTHREAD_H

#include <ringbuffer.h>
// External buffers (declared in main)
extern RingBuffer daq_buffer;
extern RingBuffer sd_buffer;
extern RingBuffer lora_buffer;

bool dispatcher_thread_step();

#endif // DISPATCHERTHREAD_H