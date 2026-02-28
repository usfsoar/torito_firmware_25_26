Keep in mind we need additional safety features for packet integridy and fail-safes in case theres a failure during transmission.

Begin testing and hardening system.

DAQ loop fails upon entering many relay commands reportedly.

Add in watchdog to restart teensy if it hangs for a while, reinitializing it.
Add in timeouts every time code interacts with a serial communication bus. Have timeout reinitialize the specific bus if it hangs for too long.