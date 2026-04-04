# Firmware: Teensy 4.1 DAQ (`env: teensy`)

[← Back to documentation index](../README.md)

## Role

The Teensy build is the **primary DAQ controller**: I2C sensor acquisition through a **TCA9548A multiplexer**, optional **SD logging**, **LoRa** transmission of sampled telemetry, and **solenoid state** reads over I2C.

**Entry point:** [`src/teensy/main.cpp`](../../src/teensy/main.cpp)

## Startup sequence

1. `Serial` at **115200** for debug.  
2. `Wire` (I2C) — Teensy 4.1 default **SDA 18 / SCL 19**.  
3. **`mux_init()`** — disables all TCA9548A channels (`lib/MuxDriver`).  
4. **`mux_select(0, ADS_MUX_CHANNEL)`** — selects the channel where the ADS1115 lives ([`hwconfig.h`](../../lib/DataTypes/hwconfig.h)).  
5. **`sensor_dispatcher_init()`** — initializes **low** and **high** pressure ADS1115 helpers (`lib/SensorDispatcher`).  
6. **`daq_init()`** — DAQ sequence/tick state (`lib/DAQLoop`).  
7. **`SDWrite::init(&sd_buffer, "data.bin")`** — optional; failure prints a warning and continues.  
8. **`LoraModule`** on **`Serial5`** (see `loramodule.h`: Teensy uses fixed `HardwareSerial&` = `Serial5`). `begin()` + `configure()` with [`lora_config.h`](../../lib/LoRaModule/lora_config.h).  
9. **`SolenoidReceive::init(SOLENOID_I2C_ADDR)`** — stores I2C address; mux selection for reads is done inside `daq_step()`.

## Main loop

- **DAQ:** roughly **20 Hz** (`next_daq += 50` ms) — calls **`daq_step()`**.  
- **`ENABLE_PRESSURE_SERIAL`** (in [`sensorconfig.h`](../../lib/DataTypes/sensorconfig.h)): when set to `1`, prints per-sensor PSI columns over USB (for bench debug).  
- **`dispatcher_thread_step()`** — moves frames from `daq_buffer` → `sd_buffer` and every 10th frame → `lora_buffer`.  
- **`sdwriter.data()`** — drains `sd_buffer` to SD.  
- **`lora_sender.send_next()`** — sends one queued LoRa packet when possible.  
- Prints **`loop dt=`** for timing diagnostics.

## Key libraries

| Library | Purpose |
|---------|---------|
| [`DAQLoop`](../../lib/DAQLoop) | Builds `SampleFrame`, mux + sensor + solenoid read |
| [`SensorDispatcher`](../../lib/SensorDispatcher) | Routes `SensorDesc` to low/high pressure readers |
| [`MuxDriver`](../../lib/MuxDriver) | TCA9548A at `0x70` |
| [`RingBuffer`](../../lib/RingBuffer) | Fixed-size queue of `SampleFrame` |
| [`BufferDispatcher`](../../lib/BufferDispatcher) | Fan-out DAQ → SD / LoRa buffers |
| [`SDWrite`](../../lib/SDWrite) | Block writes to `data.bin` + session marker |
| [`LoRaModule`](../../lib/LoRaModule) | AT-command LoRa on Teensy `Serial5` |
| [`LoraSend`](../../lib/LoraSend) | Serialize header, `AT+SEND` |
| [`SolenoidReceive`](../../lib/SolenoidReceive) | 2-byte I2C read of solenoid state |

## Configuration

- **Sensor list and count:** [`sensorconfig.h`](../../lib/DataTypes/sensorconfig.h) — **`SENSOR_COUNT`** must match table length.  
- **I2C addresses / mux channels:** [`hwconfig.h`](../../lib/DataTypes/hwconfig.h).  
- **LoRa network:** [`lora_config.h`](../../lib/LoRaModule/lora_config.h) — band, network ID, node addresses.

## See also

- [Sample frame](../reference/sample-frame.md)  
- [Ring buffers and dispatcher](../reference/ring-buffers-and-dispatcher.md)  
- [LoRa protocol notes](../protocols/lora.md)  
