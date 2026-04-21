#include "daqloop.h"

// Global state
static uint32_t seq = 0;
static uint32_t tick = 0;
static uint32_t cached_payload[SENSOR_COUNT] = {};
static uint16_t cached_raw_adc[SENSOR_COUNT] = {};
static uint8_t current_mux = 0xFF;

static bool is_ads_sensor(const SensorDesc &desc) {
    return desc.i2c_address == ADS1115_I2C_ADDR;
}

static bool select_sensor_mux(const SensorDesc &desc) {
    if (desc.mux_channel == NO_MUX) {
        return true;
    }
    if (desc.mux_channel == current_mux) {
        return true;
    }
    if (!mux_select(desc.bus_id, desc.mux_channel)) {
        return false;
    }
    current_mux = desc.mux_channel;
    return true;
}

// External buffers (declared in main)
extern RingBuffer daq_buffer;
extern SolenoidReceive solenoid_receive;

void daq_init() {
    seq = 0;
    tick = 0;
    current_mux = 0xFF;

    for (int i = 0; i < SENSOR_COUNT; ++i) {
        cached_payload[i] = 0;
        cached_raw_adc[i] = 0;
    }

    // Initialize ADC state machine
    adc_init(adc);

    if (ADS_MUX_CHANNEL != NO_MUX) {
        while (!mux_select(0, ADS_MUX_CHANNEL)) {
            Serial.println("ERROR: ADS mux select failed during DAQ init!");
            delay(1000);
        }
        current_mux = ADS_MUX_CHANNEL;
    }

    while (!adc_hw_init()) {
        Serial.println("ERROR: ADC init failed!");
        delay(1000);
    }
}

void daq_step() {
    SampleFrame frame{};              // zero-initialize entire struct (clears padding/unused fields)
    frame.seq = seq++;
    frame.timestamp_us = micros();
    frame.valid_mask = 0;
    frame.status_bits = 0;

    // Start from cached values each frame and overwrite only fresh data.
    for (int i = 0; i < SENSOR_COUNT; i++) {
        const SensorDesc &desc = sensor_table[i];
        frame.payload[desc.id] = cached_payload[desc.id];
        frame.raw_adc[desc.id] = cached_raw_adc[desc.id];
    }

    // Pipeline ADS reads within this frame: read previous, start next, then calculate previous via dispatcher.
    bool has_pending_ads = false;
    for (int i = 0; i < SENSOR_COUNT; ++i) {
        const SensorDesc &desc = sensor_table[i];

        if (!is_ads_sensor(desc)) {
            continue;
        }
        if ((tick % desc.period_ticks) != 0) {
            continue;
        }
        if (!select_sensor_mux(desc)) {
            frame.status_bits |= MUX_ERR;
            continue;
        }

        if (!has_pending_ads) {
            adc_start(desc);
            has_pending_ads = true;
            continue;
        }

        while (!adc_finish()) {
            // Busy wait is bounded by ADS1115 conversion time (~1.2ms at 860SPS).
        }

        const SensorDesc done = *adc.pending_sensor;
        const int16_t done_raw = adc.last_raw;
        adc.state = ADCPhase::Idle;
        adc.pending_sensor = nullptr;

        adc_start(desc);

        int32_t processed_value = 0;
        if (sensor_calculate_dispatch(done, done_raw, processed_value)) {
            frame.payload[done.id] = processed_value;
            frame.raw_adc[done.id] = static_cast<uint16_t>(done_raw);
            cached_payload[done.id] = processed_value;
            cached_raw_adc[done.id] = static_cast<uint16_t>(done_raw);
            frame.valid_mask |= (1 << done.id);
        } else {
            frame.status_bits |= I2C_ERR;
        }
    }

    if (has_pending_ads) {
        while (!adc_finish()) {
            // Busy wait is bounded by ADS1115 conversion time (~1.2ms at 860SPS).
        }

        if (adc.pending_sensor != nullptr) {
            const SensorDesc done = *adc.pending_sensor;
            const int16_t done_raw = adc.last_raw;
            adc.state = ADCPhase::Idle;
            adc.pending_sensor = nullptr;

            int32_t processed_value = 0;
            if (sensor_calculate_dispatch(done, done_raw, processed_value)) {
                frame.payload[done.id] = processed_value;
                frame.raw_adc[done.id] = static_cast<uint16_t>(done_raw);
                cached_payload[done.id] = processed_value;
                cached_raw_adc[done.id] = static_cast<uint16_t>(done_raw);
                frame.valid_mask |= (1 << done.id);
            } else {
                frame.status_bits |= I2C_ERR;
            }
        }
    }

    // Keep non-ADS sensors on the existing synchronous path.
    for (int i = 0; i < SENSOR_COUNT; i++) {
        const SensorDesc &desc = sensor_table[i];

        if (is_ads_sensor(desc)) {
            continue;
        }
        if ((tick % desc.period_ticks) != 0) {
            continue;
        }
        if (!select_sensor_mux(desc)) {
            frame.status_bits |= MUX_ERR;
            continue;
        }

        int32_t processed_value = 0;
        int16_t raw_adc = 0;
        if (sensor_read_dispatch(desc, processed_value, raw_adc)) {
            frame.payload[desc.id] = processed_value;
            frame.raw_adc[desc.id] = raw_adc;
            cached_payload[desc.id] = processed_value;
            cached_raw_adc[desc.id] = raw_adc;
            frame.valid_mask |= (1 << desc.id);
        } else {
            frame.status_bits |= I2C_ERR;
        }
    }

    // DAQ controls the I2C mux selection for solenoid reads (owned bus control).
    // If the mux selection fails, mark MUX_ERR and keep the last-cached solenoid value.
    // reading solenoids via SolenoidReceive
    {
        uint16_t cur = 0;
        if (SOLENOID_MUX_CHANNEL != 0xFF) {
            if (!mux_select(0, SOLENOID_MUX_CHANNEL)) {
                frame.status_bits |= MUX_ERR;
                frame.solenoid_state = solenoid_receive.get_cached_state();
            } else {
                if (!solenoid_receive.read(cur)) {
                    frame.status_bits |= I2C_ERR;
                }
                frame.solenoid_state = solenoid_receive.get_cached_state();
            }
        } else {
            if (!solenoid_receive.read(cur)) {
                frame.status_bits |= I2C_ERR;
            }
            frame.solenoid_state = solenoid_receive.get_cached_state();
        }
    }
    
    // Push to DAQ buffer (every frame)
    if (!daq_buffer.push(&frame)) {
        frame.status_bits |= OVERRUN;
    }

    tick++;
}
