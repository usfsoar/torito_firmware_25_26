#ifndef SDWRITE_H
#define SDWRITE_H

#include <SD.h>
#include <ringbuffer.h>
#include <cstring>

class SDWrite {
private:
    RingBuffer* sd_buffer = nullptr;            // pointer to the global ring buffer (set in init)
    static const size_t WRITE_BUFFER_SIZE = 4096; // bytes — keep sector-aligned (multiple of 512)
    uint8_t write_buffer[WRITE_BUFFER_SIZE];
    SampleFrame frame;
    size_t write_buffer_index = 0;
    const char *filename = "data.bin";

    enum SDMarkers {
        SD_MARKER_START = 0xA5A5A5A5,
        SD_MARKER_STOP  = 0xFFFFFFFF
    };

    // write the in-memory buffer to SD (private helper)
    bool flush_buffer();

public:
    // initialize with the ringbuffer the dispatcher pushes into
    bool init(RingBuffer* buffer, const char *fname = "data.bin");

    // call periodically from your SD task/superloop; drains sd_buffer -> write_buffer -> SD
    bool data();

    // force a flush of the internal byte buffer to SD
    bool force_flush() { return flush_buffer(); }
};


#endif // SDWRITE_H