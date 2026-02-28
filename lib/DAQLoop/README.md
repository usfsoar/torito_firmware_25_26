========
daq_init
========
Initializes the ticks and sequence varibales for the frames

========
daq_step
========
Calls the SampleFrame and initializes the variables in the frame
for loop line 28: counts through all the sensors in sensorconfig to select through them.
if statement line 37: calls for mux_selection before reading sensor data.
if statement line 47: calls sensor dispatch to read the sensor data
if statement line 57&62: Currently pushes frames to two separate buffers. Need to implement it to push into one buffer and then that daq buffer has a thread that separates the daq buffer into two other ring buffers, one for sd, one for lora.
Ends the loop incrementing tick by one