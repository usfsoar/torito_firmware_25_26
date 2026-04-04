# Roadmap and known follow-ups

[← Back to documentation index](../README.md)

Engineering notes captured from project tracking ([`NEXTSTEPS.md`](../../NEXTSTEPS.md)) and code review:

## Safety and robustness

- **Packet integrity** on LoRa (checksums / sequencing) and **fail-safe** behavior if transmission fails mid-stream.  
- **Watchdog** on Teensy to recover from hangs, reinitializing subsystems as needed.  
- **Timeouts** on all **serial / bus** interactions; on timeout, **reinitialize** the affected bus.  

## Testing

- **System testing and hardening** under realistic RF and I2C load.  
- **DAQ / relay interaction:** reports of **DAQ loop issues when many relay commands** are issued — treat as a test focus area ([`NEXTSTEPS.md`](../../NEXTSTEPS.md)).

## Documentation / config hygiene

- Align **`lora_config.h`** (Teensy) and **`lora_config_esp.h`** (ESP) for **band**, **addresses**, and any **receiver** role so OTA links match operational intent (currently differ in-repo — see [LoRa](../protocols/lora.md) and [Torito ESP](../firmware/tsesp-relay-controller.md)).

## Unit tests

The [`test/`](../../test) folder is the standard PlatformIO placeholder for future tests ([`test/README`](../../test/README)).
