=============
SDWrite Class
=============
Reason I had made this a class is to encapsulate the variables needed for the serialized write_buffer as private members.
We begin by pointing to the sd_buffer and then creating the write_buffer.
==============
enum SDMarkers
==============
Enumerators for writing session begin and session end bytes into the sdcard for future decoding and readability.

=========
bool init
=========
Takes in the pointer to the RingBuffer and an assigned name for the file to write into the SD.


=================
bool flush_buffer
=================
Private helper function that flushes the write buffer block into the sd card.

=========
bool data
=========

================
bool force_flush
================