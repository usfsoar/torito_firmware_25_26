#include "loadcell.h"

Adafruit_ADS1115 LoadCell::ads;
long LoadCell::offset_counts = 0;

bool LoadCell::init() {
    if (!LoadCell::ads.begin(ADS1115_I2C_ADDR)) {
        return false;
    }

    LoadCell::ads.setGain(GAIN_TWOTHIRDS);

    return set_zero(LoadCell::kDefaultTareSamples);
}

bool LoadCell::set_zero(uint8_t samples) {
    const uint8_t count = (samples == 0) ? 1 : samples;
    long sum = 0;

    for (uint8_t i = 0; i < count; ++i) {
        sum += LoadCell::ads.readADC_SingleEnded(LoadCell::kDefaultAdcChannel);
    }

    LoadCell::offset_counts = sum / count;
    return true;
}

bool LoadCell::read(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    const uint8_t channel = static_cast<uint8_t>(sensor.adc_channel & 0x03);

    raw_adc = LoadCell::ads.readADC_SingleEnded(channel);
    return calibrate_from_raw(sensor, raw_adc, data);
}

bool LoadCell::calibrate_from_raw(const SensorDesc &, int16_t raw_adc, int32_t &data) {
    long net_counts = static_cast<long>(raw_adc) - LoadCell::offset_counts;

    if (net_counts < 0) {
        net_counts = 0;
    }

    // Convert counts to grams with the same calibration model discussed earlier.
    // Update load_cell_rating_kg when your specific cell rating is known.
    constexpr long double load_cell_rating_kg = 1.0L;
    constexpr long double force_per_step = (load_cell_rating_kg * 1000.0L) / 4295241.0L;

    data = static_cast<int32_t>(force_per_step * static_cast<long double>(net_counts));

    return true;
}

long LoadCell::get_offset_counts() const {
    return LoadCell::offset_counts;
}