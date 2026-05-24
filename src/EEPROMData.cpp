#include "EEPROMData.h"
#include <Arduino.h>
#include <EEPROM.h>

char GetScanRateFromEEPROM() {
    uint8_t val = EEPROM.read(SAMPLERATE);
    uint8_t inv = EEPROM.read(SAMPLERATE_INV);
    if (val == 0xFF || inv != (uint8_t)~val) return -1;
    return (char)val;
}

char SetScanRateFromEEPROM(char NewRate) {
    uint8_t val = (uint8_t)NewRate;
    EEPROM.update(SAMPLERATE,     val);
    EEPROM.update(SAMPLERATE_INV, (uint8_t)~val);
    if (EEPROM.read(SAMPLERATE) != val) return -1;
    if (EEPROM.read(SAMPLERATE_INV) != (uint8_t)~val) return -1;
    return 1;
}
