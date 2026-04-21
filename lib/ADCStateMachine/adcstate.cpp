#include "adcstate.h"

ADCState adc; // global state for the ADC state machine
Adafruit_ADS1115 g_ads; // global ADS1115 instance

static uint16_t ads_mux_from_channel(uint8_t channel) {
    switch (channel & 0x03) {
        case 0:
            return ADS1X15_REG_CONFIG_MUX_SINGLE_0;
        case 1:
            return ADS1X15_REG_CONFIG_MUX_SINGLE_1;
        case 2:
            return ADS1X15_REG_CONFIG_MUX_SINGLE_2;
        default:
            return ADS1X15_REG_CONFIG_MUX_SINGLE_3;
    }
}

void adc_init(ADCState &adc_state) {
    adc_state.state = ADCPhase::Idle;
    adc_state.pending_sensor = nullptr;
    adc_state.pending_channel = 0;
    adc_state.pending_mux = NO_MUX;
    adc_state.start_us = 0;
    adc_state.last_raw = 0;
}

bool adc_hw_init() {
    if (!g_ads.begin(ADS1115_I2C_ADDR)) {
        return false;
    }

    g_ads.setGain(GAIN_TWOTHIRDS);            // keep your current gain choice
    g_ads.setDataRate(RATE_ADS1115_860SPS);   // max ADS1115 rate
    return true;
}

void adc_start(const SensorDesc &desc) {
    // use non-blocking ADS1115 API
    g_ads.startADCReading(ads_mux_from_channel(desc.adc_channel), false); // single-shot mode
    adc.state = ADCPhase::Converting;
    adc.pending_sensor = &desc;
    adc.pending_channel = desc.adc_channel;
    adc.pending_mux = desc.mux_channel;
    adc.start_us = micros();
}

bool adc_finish() {
    if (adc.state != ADCPhase::Converting) return false;

    // wait enough time for conversion to complete
    if (micros() - adc.start_us < ADS1115_CONV_US) return false;

    adc.last_raw = g_ads.getLastConversionResults();
    adc.state = ADCPhase::ResultReady;
    return true;
}