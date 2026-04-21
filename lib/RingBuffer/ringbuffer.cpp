#include "ringbuffer.h"

RingBuffer::RingBuffer() : head(0), tail(0), count(0), overrun_count(0) {
    // Constructor - initialize members
}

bool RingBuffer::push(const SampleFrame *frame) {
    uint16_t next = (head + 1) & mask;

    if (next == tail) {
        overrun_count++;
        return false;
    }
    frames[head] = *frame;
    head = next;
    
    return true;
}

bool RingBuffer::pop(SampleFrame *frame) {
    if (head == tail) {
        return false;
    }
    *frame = frames[tail];
    tail = (tail + 1) & mask;
    return true;
}

uint16_t RingBuffer::get_count() const {
    return (head - tail) & mask;
}

uint32_t RingBuffer::get_overrun_count() const {
    return overrun_count;
}

void RingBuffer::reset_overrun_count() {
    overrun_count = 0;
}