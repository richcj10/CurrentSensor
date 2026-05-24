#include <Arduino.h>
#include "ModBusBL.h"
#include "Define.h"
#include "Sensor.h"
#include "EEPROMData.h"

ModBusBL modbus(DIR, 2, 6, 4, 0);

unsigned long previousMillis    = 0;
unsigned long previousLEDMillis = 0;
unsigned int  SensorTestTime    = SENSORSCANRATE;

void ComunicationUpdate() {
    modbus.update();

    // Handle holding register commands
    uint16_t cmd = modbus.getHolding(0);
    if (cmd == SENSOR_CHANGE_SCANRATE) {
        SensorTestTime = modbus.getHolding(1);
        SetScanRateFromEEPROM((char)modbus.getHolding(1));
        modbus.setHolding(0, 0);
    }

    // Update live input registers
    modbus.setInput(2, (uint16_t)(GetSensorValues(SENSOR_INPUT_VIN) * 100));
    modbus.setInput(3, (uint16_t)(GetSensorValues(SENSOR_INPUT_1)   * 100));
    modbus.setInput(4, (uint16_t)(GetSensorValues(SENSOR_INPUT_2)   * 100));
    modbus.setInput(5, (uint16_t)(GetSensorValues(SENSOR_INPUT_3)   * 100));
}

void setup() {
    pinMode(LED,   OUTPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(LED,   LOW);
    digitalWrite(RELAY, LOW);

    modbus.begin(38400);

    // Static identity registers — set once
    modbus.setInput(0, CURRENTSENSORVERSION);
    modbus.setInput(1, CURRENTSENSORTYPE);
    modbus.setCoil(SENSOR_SCAN_EN, true); // Start scanning by default

    // Restore scan rate from EEPROM; rewrite default if missing or corrupt
    char rate = GetScanRateFromEEPROM();
    if (rate > 0) {
        SensorTestTime = (unsigned int)rate;
    } else {
        SetScanRateFromEEPROM((char)SENSORSCANRATE);
    }

    SensorStart();
}

void loop() {
    unsigned long currentMillis = millis();

    ComunicationUpdate();

    if (modbus.getCoil(SENSOR_SCAN_EN)) {
        if (currentMillis - previousMillis >= SensorTestTime * SENSORTESTTIMEMULTIPLYER) {
            previousMillis = currentMillis;
            ReadSensors();
        }
    }

    //if (modbus.getCoil(SENSOR_LED_CONTROL)) {
        if (currentMillis - previousLEDMillis >= 500) {
            previousLEDMillis = currentMillis;
            digitalWrite(LED, !digitalRead(LED));
        }
    //}

    // Master controls relay directly via SENSOR_RELAY coil
    digitalWrite(RELAY, modbus.getCoil(SENSOR_RELAY));
}
