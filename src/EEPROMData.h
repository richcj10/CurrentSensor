#ifndef EEPROMDATA_H
#define EEPROMDATA_H

/* Application EEPROM layout — must not overlap MBBP reserved bytes:
 *   0x00  MBBP_EE_BOOT_FLAG
 *   0x01  MBBP_EE_SLAVE_ID      (managed by ModBusBL)
 *   0x02  MBBP_EE_APP_VALID
 *   0x03  MBBP_EE_SENTINEL
 *   0x04  MBBP_EE_SLAVE_ID_INV
 * App data starts at 0x05.
 * Each value is paired with its bitwise-inverse for corruption detection. */
#define SAMPLERATE     0x05   /* scan rate in seconds        */
#define SAMPLERATE_INV 0x06   /* ~SAMPLERATE for validation  */

/* Returns scan rate (1-255) on valid read, -1 if unset or corrupt. */
char GetScanRateFromEEPROM();

/* Writes scan rate + inversion byte. Returns 1 on success, -1 on verify failure. */
char SetScanRateFromEEPROM(char NewRate);

#endif
