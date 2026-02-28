// ringbuffer.h
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <SampleFrame.h>

#define RING_BUFFER_SIZE 255

class RingBuffer {
private:
    SampleFrame frames[RING_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
    uint32_t overrun_count;

public:
    RingBuffer();
    bool push(const SampleFrame *frame);
    bool pop(SampleFrame *frame);
    uint16_t get_count() const;
    uint32_t get_overrun_count() const;
    void reset_overrun_count();
};

#endif // RINGBUFFER_H