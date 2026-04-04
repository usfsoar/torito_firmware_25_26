# Libraries (`lib/`)

[← Back to documentation index](../README.md)

PlatformIO compiles each subfolder under [`lib/`](../../lib) as a project library. The table below maps folders to responsibilities; **each folder has a `README.md`** (or the root [`lib/README`](../../lib/README) index) with purpose, primary files, and deep links back here.

| Library | Role | README |
|---------|------|--------|
| **BufferDispatcher** | `daq_buffer` → `sd_buffer` / `lora_buffer` | [README](../../lib/BufferDispatcher/README.md) |
| **DAQLoop** | `daq_init` / `daq_step`, `SampleFrame` acquisition | [README](../../lib/DAQLoop/README.md) |
| **DataTypes** | `SampleFrame`, `SensorDesc`, `sensorconfig`, `hwconfig` | [README](../../lib/DataTypes/README.md) |
| **ESPLoRaModule** | ESP32 `LoRaModule` + `lora_config_esp.h` | [README](../../lib/ESPLoRaModule/README.md) |
| **HighPressure** | ADS1115 high-pressure scaling | [README](../../lib/HighPressure/README.md) |
| **I2CPrimitives** | `i2c_write` / `i2c_read` / `i2c_write_read` | [README](../../lib/I2CPrimitives/README.md) |
| **LoRaModule** | Teensy AT LoRa (`Serial5`) + `lora_config.h` | [README](../../lib/LoRaModule/README.md) |
| **LoraSend** | Header serialization, LoRa send, backoff | [README](../../lib/LoraSend/README.md) |
| **LowPressure** | ADS1115 low-pressure scaling | [README](../../lib/LowPressure/README.md) |
| **MuxDriver** | TCA9548A @ `0x70` | [README](../../lib/MuxDriver/README.md) |
| **RingBuffer** | `SampleFrame` queue | [README](../../lib/RingBuffer/README.md) |
| **SDWrite** | SD writer for `data.bin` | [README](../../lib/SDWrite/README.md) |
| **SensorDispatcher** | Init + dispatch by sensor type | [README](../../lib/SensorDispatcher/README.md) |
| **SolenoidReceive** | 2-byte I2C solenoid state | [README](../../lib/SolenoidReceive/README.md) |
| **Temperature** | ADS1115 °C path; not in dispatcher yet | [README](../../lib/Temperature/README.md) |

**Index of all libraries:** [`lib/README`](../../lib/README)

## External dependencies (Teensy only)

From [`platformio.ini`](../../platformio.ini) `env:teensy`:

- `adafruit/Adafruit ADS1X15`
- `adafruit/Adafruit BusIO`

## See also

- [System overview](../architecture/system-overview.md)  
- [Sample frame](../reference/sample-frame.md)  
