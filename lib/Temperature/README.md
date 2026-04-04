# Temperature

**Purpose:** **Temperature-style** ADS1115 scaling (`SensorDesc::TEMPERATURE`): voltage → °C via configured min/max voltage span.

**Primary files:** `temperature.h`, `temperature.cpp`

**Status:** Not referenced in the current `sensor_read_dispatch()` switch in [`SensorDispatcher`](../SensorDispatcher/sensordispatcher.cpp) (only `LOWPRESSURE` and `HIGHPRESSURE`). Extend the dispatcher if you add temperature rows to [`sensorconfig.h`](../DataTypes/sensorconfig.h).

**Documentation:** [Library index](../../docs/libraries/index.md)
