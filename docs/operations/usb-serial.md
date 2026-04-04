# USB serial operations

[← Back to documentation index](../README.md)

All three environments use **115200** baud on the USB CDC serial port unless you change `Serial.begin` in code.

## Teensy (`teensy`)

- **Boot messages:** mux/sensor init, SD status, LoRa warnings, `DAQ Ready!`.  
- **`loop dt=`** — time between iterations of `loop()` (ms); use to spot scheduling pressure.  
- **`ENABLE_PRESSURE_SERIAL`** in [`sensorconfig.h`](../../lib/DataTypes/sensorconfig.h): when **`1`**, prints one tab-separated line of PSI values per DAQ frame (sensor order follows `sensor_table`). When **`0`**, that block is compiled out.

## GSC ESP (`gscesp`)

- **Host → device:** lines like **`<hex_or_ascii_payload>,<destination_address>`** (see [GSC ESP firmware](../firmware/gscesp-lora-bridge.md)).  
- **Append `:repeat`** to enable alternating test payloads once per second.  
- **Device → host:** binary prefix **`0xAA 0x55`** then raw payload bytes after a **`+RCV`** parse on RX LoRa.

## Torito ESP (`tsesp`)

- **Debug:** startup banner, LoRa configure success/failure, I2C slave address.  
- Use serial mainly for bring-up; relay control is via **LoRa** and **I2C** writes.
