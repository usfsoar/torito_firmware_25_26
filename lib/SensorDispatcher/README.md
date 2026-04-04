# SensorDispatcher

**Purpose:** Initialize **low** and **high** pressure ADS1115 front-ends and **route** a read by `SensorDesc::sensorType`.

**Primary files:** `sensordispatcher.h`, `sensordispatcher.cpp`

**API:** `sensor_dispatcher_init()`, `sensor_read_dispatch(desc, processed, raw_adc)`

**Used by:** Teensy DAQ (`daqloop.cpp`).

**Documentation:** [Teensy DAQ](../../docs/firmware/teensy-daq.md) · [Library index](../../docs/libraries/index.md)
