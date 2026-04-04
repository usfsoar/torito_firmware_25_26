# DataTypes

**Purpose:** Shared **data layout** and **hardware/sensor configuration** headers used across DAQ, buffers, and telemetry.

**Files:**

| File | Role |
|------|------|
| `SampleFrame.h` | Binary DAQ record: timestamp, seq, masks, `payload[]`, `raw_adc[]`, `solenoid_state` |
| `SensorDesc.h` | Per-sensor descriptor: type, mux, I2C, ADC channel, `period_ticks` |
| `sensorconfig.h` | `SENSOR_COUNT`, sensor table, `ENABLE_PRESSURE_SERIAL` |
| `hwconfig.h` | Solenoid I2C address, ADS1115 address, mux channels |

**Notes:** `valid_mask` uses **one bit per sensor id** (not “two bytes” — the struct uses `uint8_t`). Keep `SENSOR_COUNT` aligned with the sensor table length.

**Used by:** Teensy stack and any code including `SampleFrame` / `sensorconfig.h`.

**Documentation:** [Sample frame](../../docs/reference/sample-frame.md) · [Configuration headers](../../docs/reference/configuration-headers.md) · [Library index](../../docs/libraries/index.md)
