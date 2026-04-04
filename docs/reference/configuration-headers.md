# Configuration headers

[← Back to documentation index](../README.md)

Central places to edit behavior **without** scattering magic numbers:

## Teensy / shared `lib/DataTypes`

| File | Purpose |
|------|---------|
| [`hwconfig.h`](../../lib/DataTypes/hwconfig.h) | Solenoid I2C address, ADS1115 address, mux channel indices |
| [`sensorconfig.h`](../../lib/DataTypes/sensorconfig.h) | `SENSOR_COUNT`, sensor table, `ENABLE_PRESSURE_SERIAL` |
| [`SensorDesc.h`](../../lib/DataTypes/SensorDesc.h) | Sensor descriptor shape |
| [`SampleFrame.h`](../../lib/DataTypes/SampleFrame.h) | DAQ record layout |

## Teensy LoRa

| File | Purpose |
|------|---------|
| [`lora_config.h`](../../lib/LoRaModule/lora_config.h) | Baud, band, network ID, node addresses, AT templates, `RELAY_MSB_BIT`, PHY defaults |

## ESP LoRa (relay)

| File | Purpose |
|------|---------|
| [`lora_config_esp.h`](../../lib/ESPLoRaModule/lora_config_esp.h) | Same role as Teensy `lora_config.h` for `lib/ESPLoRaModule` |

## Mux

| File | Purpose |
|------|---------|
| [`muxdriver.h`](../../lib/MuxDriver/muxdriver.h) | TCA9548A address **`0x70`** |

## PlatformIO

[`platformio.ini`](../../platformio.ini) — board, **`build_src_filter`**, UART/COM ports, Teensy `lib_deps`.

## Cross-checklist when changing hardware

1. **`SENSOR_COUNT`** matches `sensor_table[]` length.  
2. **Mux channels** match PCB wiring (ADS vs solenoid path).  
3. **LoRa:** band, network ID, addresses, and `AT+PARAMETER` match on **both** ends.  
4. **Relay ESP** I2C address **`0x09`** matches **`SOLENOID_I2C_ADDR`** on Teensy if sharing the same logical device.
