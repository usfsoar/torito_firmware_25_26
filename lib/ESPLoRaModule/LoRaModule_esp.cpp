#include "LoRaModule_esp.h"
#include "lora_config_esp.h"

LoRaModule::LoRaModule(uint8_t rxPin, uint8_t txPin, uint8_t address) 
    : _rxPin(rxPin), _txPin(txPin), _address(address) {
    // ESP32: use Serial1 instance; pins will be configured in begin()
    loraSerial = &Serial1;
}

bool LoRaModule::begin() {
    // Configure Serial1 for the LoRa UART using the pins supplied to the constructor
    loraSerial->begin(115200, SERIAL_8N1, _rxPin, _txPin);
    delay(1000);

    if (sendATCommand("AT").indexOf("OK") != -1) {
        return true;
    } else {
        return false;
    }
}

bool LoRaModule::configure(uint8_t address, unsigned long band, uint8_t networkId) {
    char cmd[50];
    
    // Set address
    sprintf(cmd, "AT+ADDRESS=%d", address);
    sendATCommand(cmd);
    delay(100);
    
    // Set band
    sprintf(cmd, "AT+BAND=%lu", band);
    sendATCommand(cmd);
    delay(100);
    
    // Set network ID
    sprintf(cmd, "AT+NETWORKID=%d", networkId);
    sendATCommand(cmd);
    delay(LORA_CONFIG_DELAY);

    // Apply default radio parameters (SF, BW, CR, preamble)
    if (setParameter(LORA_PARAMETER_SF, LORA_PARAMETER_BW, LORA_PARAMETER_CR, LORA_PARAMETER_PREAMBLE)) {
        // parameters set
    } else {
        // failed to set parameters
    }
    return true;
}

String LoRaModule::sendATCommand(const char* command, unsigned long timeout) {
    String result = "";
    while (loraSerial->available()) {
        loraSerial->read();
    }

    // record the AT command so any later "+ERR=" lines can be correlated
    _lastATCommand = String(command);
    loraSerial->println(command);

    unsigned long startTime = millis();
    unsigned long lastCharTime = millis();
    while (millis() - startTime < timeout) {
        if (loraSerial->available()) {
            char c = loraSerial->read();
            result += c;
            lastCharTime = millis();
            if ((result.indexOf("OK") != -1 || result.indexOf("ERROR") != -1) && 
                millis() - lastCharTime > 50) {
                break;
            }
        }
    }
    return result;
}

bool LoRaModule::sendData(uint8_t destAddress, String hexData) {
    char cmd[100];
    sprintf(cmd, "AT+SEND=%d,%d,%s", destAddress, hexData.length() / 2, hexData.c_str());
    String response = sendATCommand(cmd, 2000);
    return response.indexOf("OK") != -1;
}

bool LoRaModule::setParameter(uint8_t sf, uint8_t bw, uint8_t cr, uint8_t preamble) {
    char cmd[64];
    sprintf(cmd, AT_SET_PARAMETER_FMT, sf, bw, cr, preamble);
    String resp = sendATCommand(cmd, AT_COMMAND_TIMEOUT);
    delay(LORA_CONFIG_DELAY);
    return resp.indexOf("OK") != -1;
}

bool LoRaModule::receiveData(String& hexData) {
    if (!loraSerial->available()) {
        return false;
    }
    String incomingString = "";
    // Read with timeout
    unsigned long startTime = millis();
    unsigned long lastCharTime = millis();
    while (millis() - startTime < 1000) {
        if (loraSerial->available()) {
            char c = loraSerial->read();
            if (c == '\n') break;
            incomingString += c;
            lastCharTime = millis();
        } else if (millis() - lastCharTime > 50) {
            // No data for 50ms, likely complete
            break;
        }
    }

    incomingString.trim();
    // error handling logic removed for release

    // Parse format: +RCV=<address>,<length>,<data>,<RSSI>,<SNR>
    int firstComma = incomingString.indexOf(',');
    int secondComma = incomingString.indexOf(',', firstComma + 1);
    
    if (firstComma > 0 && secondComma > 0) {
        hexData = incomingString.substring(secondComma + 1);
        // Remove RSSI and SNR if present
        int thirdComma = hexData.indexOf(',');
        if (thirdComma > 0) {
            hexData = hexData.substring(0, thirdComma);
        }
        hexData.trim();
        return true;
    }
    
    return false;
}
