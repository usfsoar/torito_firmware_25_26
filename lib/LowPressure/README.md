====================
PressureSensor::init
====================
Member function whose job is to initialize the PressureSensor class and get it to point to the correct i2c_address

!!!Currently it is hardcoded and will need to be rewritten to accept the address from sensorconfig. This will require a bit of changing in sensor_dispatcher!!!

====================
PressureSensor::read
====================
raw_adc: uses adafruit library to read from the adc using the adc.channel variable.
Line 20-23 is calibration formula
data: converts the float back into an int to save bandwidth inside the frame where the data is stored.

!!! The if-statement regarding the raw_adc check needs to be a bit more robust, will look into later, but works for now!!!