The reason we are using ring buffers is to separate consumers/producers once we integrate threads/tasks. This helps reduce contention when paired with a single-producer/single-consumer design or RTOS synchronization. There will be a total of three buffers in this program when we switch to RTOS. One main buffer that the hardware pushes frames into first, and then a dispatcher thread that sends frames from that buffer into an sd_card buffer and a LoRa buffer.
Reason being that the sd_card buffer may stall during writes and we want to avoid those stalls from impacting the LoRa sends.

========================
RingBuffer::RingBuffer()
========================
This is the initial constructor for the class. It establishes the data member list.

head: pointer that writes to the buffer. It cannot be allowed to pass the tail.
tail: pointer that reads from the buffer. It cannot be allowed to pass the head.
count: holds the amount of frames within the buffer. The head will increment the counter by one, the tail decrements it by one. It more so represents the number of frames left unread within the buffer.
overrun_count: variable that holds the amount of times the buffer is called to push a new frame in, but the buffer is full and rejects the new frame trying to be pushed in. Essentially counts the number of frames that overflow. We are currently dropping newest frames rather than oldest.

================
RingBuffer::push
================
Main purpose is to push(insert) frames into the buffer.
Takes the address of the caller's frame and copies it into the buffer.
Once this is done, the head pointer increments to the next index by one and increments the count. If the head pointer hits the RING_BUFFER_SIZE it will wrap back around to 0. (this is done via modulo currently).

===============
RingBuffer::pop
===============
Main purpose is to pop(remove) frames from the buffer.
If count equals 0, it returns false indicating that there are no frames in the buffer to pop.
Copies the oldest frame from the buffer into the caller's frame and advances the tail index.
Tail then increments to the next index by one and decrements the count. If the tail pointer hits the RING_BUFFER_SIZE it will wrap back around to 0. (this is done via modulo currently).

=====================================================
get_count()/get_overrun_count()/reset_overrun_count()
=====================================================
Should be self explanatory, simple member functions that return the variables within the class.