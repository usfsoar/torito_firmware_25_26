#include "lorasend.h"

bool LoraSend::init(LoraModule *lora_mod, RingBuffer *rb, uint8_t dest_address_) {
    if (!lora_mod || !rb) {
        return false;
    }
    lora_module = lora_mod;
    ring_buffer = rb;
    dest_address = dest_address_;
    return true;
}

bool LoraSend::send_next() {
    // Basic guards
    if (!ring_buffer || !lora_module) return false;

    // Respect short backoff after a failure to avoid repeated blocking attempts
    if (millis() < next_allowed_send_ms) {
        // quietly honor backoff without spamming the serial
        return false;
    }

    // Nothing to send
    size_t count = ring_buffer->get_count();
    if (count == 0) {
        // no data queued
        return false;
    }

    // If module is currently not known-online, try a probe after backoff
    if (!lora_module->is_online()) {
        unsigned long now = millis();
        if (now < next_allowed_send_ms) {
            Serial.print("LoraSend: module offline, waiting until ");
            Serial.println(next_allowed_send_ms);
            return false;
        }
        // backoff expired; attempt a lightweight ping
        Serial.println("LoraSend: probing offline module");
        if (!lora_module->ping(200)) {
            Serial.println("LoraSend: ping failed, extending backoff");
            next_allowed_send_ms = now + 5000;
            return false;
        }
        // ping succeeded; continue and send the queued packet
        Serial.println("LoraSend: module responsive again");
    }

    // Pop one frame and attempt send
    SampleFrame frame;
    if (!ring_buffer->pop(&frame)) {
        Serial.println("LoraSend: pop failed despite nonzero count");
        return false;
    }

    // serialize into a fixed-size stack buffer (headroom kept by LoraSend::MAX_SERIALIZED_HEADER)
    uint8_t buf[MAX_SERIALIZED_HEADER];
    size_t len = serialize_frame_header(frame, buf, sizeof(buf));
    if (len == 0) {
        Serial.println("LoraSend: serialization failure");
        return false;
    }

    // print the actual bytes we are about to send (hex string)
    Serial.print("LoraSend: data=");
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] < 0x10) Serial.print('0');
        Serial.print(buf[i], HEX);
    }
    Serial.println();

    // Try to send; on failure requeue and set backoff to avoid spinning
    if (lora_module->send_data_hexstr(dest_address, buf, len)) {
        // success
        next_allowed_send_ms = 0;
        return true;
    } else {
        // push frame back for retry later
        (void)ring_buffer->push(&frame);
        next_allowed_send_ms = millis() + 5000; // 5s backoff
        Serial.println("LoraSend: send failed, requeued");
        // hex dump already printed above, queue count may help diagnose
        Serial.print("Queue count=");
        Serial.println(ring_buffer->get_count());
        return false;
    }
}

size_t LoraSend::send_all() {
    if (!ring_buffer || !lora_module) return 0;
    size_t sent = 0;
    SampleFrame frame;
    while (ring_buffer->pop(&frame)) {
        // reuse the same stack buffer for each packet
        uint8_t buf[MAX_SERIALIZED_HEADER];
        size_t len = serialize_frame_header(frame, buf, sizeof(buf));
        if (len == 0) break;
        if (lora_module->send_data_hexstr(dest_address, buf, len)) {
            ++sent;
        } else {
            // stop on failure to avoid spinning on a bad link
            break;
        }
    }
    return sent;
}

// Pack header fields tightly (no compiler padding) in little-endian order.
size_t LoraSend::serialize_frame_header(const SampleFrame &frame, uint8_t *out, size_t out_len) {
    // required size: sensor_count(1) + timestamp(4) + seq(4) + valid_mask(1) + status_bits(1) + solenoid_state(2) +
    // raw_adc[SENSOR_COUNT] (2 bytes each)
    const size_t required = 1 + 4 + 4 + 1 + 1 + sizeof(frame.solenoid_state) + (sizeof(frame.raw_adc));
    if (out_len < required || required > MAX_SERIALIZED_HEADER) return 0;

    size_t pos = 0;
    out[pos++] = static_cast<uint8_t>(SENSOR_COUNT);

    // 32-bit little-endian
    out[pos++] = (uint8_t)(frame.timestamp_us & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 8) & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 16) & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 24) & 0xFF);

    out[pos++] = (uint8_t)(frame.seq & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 8) & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 16) & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 24) & 0xFF);

    out[pos++] = frame.valid_mask;
    out[pos++] = frame.status_bits;

    // solenoid_state as little-endian uint16_t
    {
        uint16_t s = frame.solenoid_state;
        out[pos++] = (uint8_t)(s & 0xFF);
        out[pos++] = (uint8_t)((s >> 8) & 0xFF);
    }

    // raw_adc[] as little-endian uint16_t
    for (size_t i = 0; i < SENSOR_COUNT; ++i) {
        uint16_t v = frame.raw_adc[i];
        out[pos++] = (uint8_t)(v & 0xFF);
        out[pos++] = (uint8_t)((v >> 8) & 0xFF);
    }
    return pos;
}