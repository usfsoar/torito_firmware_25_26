# HighPressure

**Purpose:** **High-pressure** channel support: ADS1115 single-ended read and voltage → PSI scaling (higher `PSI_MAX` than `LowPressure`).

**Primary files:** `highpressure.h`, `highpressure.cpp`

**Used by:** `SensorDispatcher` when `SensorDesc::sensorType` is `HIGHPRESSURE` (see [`sensorconfig.h`](../DataTypes/sensorconfig.h)).

**Documentation:** [Library index](../../docs/libraries/index.md) · [Sample frame](../../docs/reference/sample-frame.md)
