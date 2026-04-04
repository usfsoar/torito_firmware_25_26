# `lib/` — project libraries

PlatformIO compiles each **subfolder** here as a **private static library** and links it into the firmware. The [Library Dependency Finder](https://docs.platformio.org/page/librarymanager/ldf.html) picks up includes from your `src/` and `lib/` sources automatically.

## Full documentation

**Architecture, build, protocols, and cross-library behavior:** [`../docs/README.md`](../docs/README.md)  
**This folder at a glance:** [`../docs/libraries/index.md`](../docs/libraries/index.md)

## Libraries (A–Z)

| Folder | Role | README |
|--------|------|--------|
| [BufferDispatcher](BufferDispatcher/README.md) | Fan-out: `daq_buffer` → `sd_buffer` / `lora_buffer` | ✓ |
| [DAQLoop](DAQLoop/README.md) | `daq_init` / `daq_step`, builds `SampleFrame` | ✓ |
| [DataTypes](DataTypes/README.md) | `SampleFrame`, `SensorDesc`, `sensorconfig`, `hwconfig` | ✓ |
| [ESPLoRaModule](ESPLoRaModule/README.md) | ESP32 LoRa AT driver + `lora_config_esp.h` | ✓ |
| [HighPressure](HighPressure/README.md) | ADS1115 high-pressure scaling | ✓ |
| [I2CPrimitives](I2CPrimitives/README.md) | Thin `Wire` read/write helpers | ✓ |
| [LoRaModule](LoRaModule/README.md) | Teensy LoRa AT driver + `lora_config.h` | ✓ |
| [LoraSend](LoraSend/README.md) | Serialize header, send via LoRa | ✓ |
| [LowPressure](LowPressure/README.md) | ADS1115 low-pressure scaling | ✓ |
| [MuxDriver](MuxDriver/README.md) | TCA9548A I2C mux | ✓ |
| [RingBuffer](RingBuffer/README.md) | Fixed-size `SampleFrame` queue | ✓ |
| [SDWrite](SDWrite/README.md) | SD batch writer (`data.bin`) | ✓ |
| [SensorDispatcher](SensorDispatcher/README.md) | Dispatch reads by sensor type | ✓ |
| [SolenoidReceive](SolenoidReceive/README.md) | I2C read of 16-bit solenoid state | ✓ |
| [Temperature](Temperature/README.md) | ADS1115 temperature path (optional type) | ✓ |

## Layout convention

Place each library in its own directory with headers/sources at the top level (as in this project), or under `src/` if you prefer PlatformIO’s nested layout. Optional: `library.json` for custom build flags — see [PlatformIO library config](https://docs.platformio.org/page/librarymanager/config.html).

## Example

Firmware includes project headers by name, for example:

```cpp
#include <daqloop.h>
#include <ringbuffer.h>
```

---

*Torito firmware — see [`../docs/`](../docs/README.md) for the full documentation tree.*
