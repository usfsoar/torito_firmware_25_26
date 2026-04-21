#ifndef ADC_STATE_H
#define ADC_STATE_H

#include <SensorDesc.h>
#include <hwconfig.h>
#include <muxdriver.h>
#include <Adafruit_ADS1X15.h>

#define ADS1115_CONV_US 1200 // microseconds for ADS1115 conversion at 860SPS

enum class ADCPhase {
    Idle,
    Converting,
    ResultReady,
    Error
};

struct ADCState {
    ADCPhase state = ADCPhase::Idle;
    const SensorDesc *pending_sensor = nullptr;
    uint8_t pending_channel = 0;
    uint8_t pending_mux = NO_MUX;
    uint32_t start_us = 0;
    int16_t last_raw = 0;
};

void adc_init(ADCState &adc_state);
void adc_start(const SensorDesc &desc);
bool adc_finish();
bool adc_hw_init();
extern ADCState adc; // global state for the ADC state machine
extern Adafruit_ADS1115 g_ads; // global ADS1115 instance

#endif // ADC_STATE_H
