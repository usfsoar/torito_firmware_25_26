================
LoraSend::init()
================

Makes sure lora_module and ring_buffer for lora are initialized for setup and then links the buffer and lora module to the class.

=====================
LoraSend::send_next()
=====================

Prepares the next frame to send over the lora

====================
LoraSend::send_all()
====================

Sends out the whole buffer ove rthe lora flushing it

================================
LoraSend::serialize_frame_header
================================

Takes the frame data and converts it into hexstring to prepare for a send over the lora.