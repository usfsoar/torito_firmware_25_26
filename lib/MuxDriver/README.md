========
mux_init
========
All this does is initialize the mux and get it to point to a channel (channel 0) on startup.
It gets called in main.
Returns a bool to signal whether it succeeded or not.

==========
mux_select
==========
Currently bus_id is not being used.
channel is being passed and used to specify the mask for the mux.
The mask is a byte that specifies the channel to use on the mux and gets the mux to point to it with i2c_write.