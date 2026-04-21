#ifndef LORA_MODULE_H_ESP
#define LORA_MODULE_H_ESP

#include <Arduino.h>
#include <HardwareSerial.h>

class LoRaModule {
public:
    LoRaModule(uint8_t rxPin, uint8_t txPin, uint8_t address);
    LoRaModule(HardwareSerial& serialPort, uint8_t rxPin, uint8_t txPin, uint8_t address);
    
    bool begin();
    bool configure(uint8_t address, unsigned long band, uint8_t networkId);
    bool setParameter(uint8_t sf, uint8_t bw, uint8_t cr, uint8_t preamble); // AT+PARAMETER
    String sendATCommand(const char* command, unsigned long timeout = 1000);
    bool sendData(uint8_t destAddress, String hexData);
    bool receiveData(String& hexData);
    
private:
    HardwareSerial* loraSerial = &Serial1;
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _address;

    // store the last AT command sent so receive-time errors (+ERR=) can show context
    String _lastATCommand;
};

#endif
