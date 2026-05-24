#ifndef EEPROMDATA_H
#define EEPROMDATA_H

/* EEPROM layout — must not overlap MBBP reserved bytes:
 *   0x00 MBBP_EE_BOOT_FLAG
 *   0x01 MBBP_EE_SLAVE_ID  (managed by ModBusBL)
 *   0x02 MBBP_EE_APP_VALID
 * App data starts at 0x03. */
#define SAMPLERATE 0x03

char GetScanRateFromEEPROM();
char SetScanRateFromEEPROM(char NewRate);

#endif
