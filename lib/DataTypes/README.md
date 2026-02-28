===========
SampleFrame
===========
timestamp_us: records the time the frame is initially being made
seq: the index of the frame
valid_mask: uses 2 bytes to indicate which sensors send back
valid data by setting bits within the bytes.
    Ex: Say we have 4 sensors running. If all sensors are good
        valid_mask would contain 0x000F. (1111)
        Let's say that during a read, sensor 2 fails
        valid_mask would contain 0x000D. (1101)
        A set bit means the sensor is working.
status_bits: Will contain specific error codes for various
    states, such as: mux failing resulting in MUX_ERR,
    a sensor failing resulting in an I2C_ERR,
    a buffer overrun resulting in OVERRUN.
    Each error has a specific binary code.
payload[]: Contains all calibrated sensor data for each
    specific sensor called during the frame's lifetime
raw_adc[]: contains all uncalibrated sensor data for each
    specific sensor called during the frame's lifetime.
    Important for redundancy in case calibration formulas
    within sensor classes are off.
static_asserts: Checking during compile time that the struct contain valid
    data types for storage. Meaning, no dynamic memory types, no user-defined constructors/destructors or virtual functions and no inheritence.

==========
SensorDesc
==========
This is a struct to contain specific sensor information.
The rest of the code will call specific variables here for
that sensor.
id: the index for the sensor, used in arrays for code
enum type - sensorType: enumerator for sensor types, add new
    sensor types here. Specifically used to give a meaningful
    name to sensor types rather than a list of 0-9.
bus_id: The bus the sensor is attached to. Mainly here for
    future proofing when we add in new muxes.
mux_channel: The channel on the mux that connects to the mux.
i2c_address: The address of the sensor on the mux.
adc_channel: The channel to communicate to the adc.
period_ticks: The period in which the sensor is read in the
    DAQLoop. If the loop is running at 20Hz, then a sensor
    with a tick of 1 is read every 20Hz (50ms). A sensor with
    a period_tick of 5 is read every 250 ms.
    This is to abstract timing within the program.

============
sensorconfig
============
Area to add in sensors and adjust the configuration of each
sensor. Always make sure to adjust SENSOR_COUNT to match number
of configured sensors and vice versa. Simple enough.

========
hwconfig
========
Used to label pinouts, no real use yet, mainly here for
future implementation