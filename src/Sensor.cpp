#include "Sensor.h"
#include "Define.h"
#include <Arduino.h>
#include <avr/delay.h>

// ── Configurable thresholds ───────────────────────────────────────────────────
unsigned int SenorWaterFault            = 550;
unsigned int SenorDisconectedThreshold  = 600;
int          SenorVoltageHighThreshold  = 800;
int          SenorVoltageLowThreshold   = 270;

// ── Sensor state ──────────────────────────────────────────────────────────────
float CurrentSenor[4] = {0, 0, 0, 0};
char  RelayState = 0;

// ── Init ──────────────────────────────────────────────────────────────────────

void adcinit() {
    // AVCC reference, right-adjusted, ADC0 selected
    ADMUX = 0b01000000;
    // Enable ADC, prescaler /128
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
}

void SensorStart() {
    pinMode(SENSORPWR, OUTPUT);
    digitalWrite(SENSORPWR, LOW);
    adcinit();
    // Warm up ADC — discard first readings after power-on
    for (uint8_t i = 0; i < 10; i++) adcread(ADC_VIN);
}

// ── Low-level ADC read (direct AVR register access) ───────────────────────────

int adcread(unsigned char channel) {
    ADMUX = 0x40 | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    _delay_ms(1);
    return ADCW;
}

// ── Acquisition ───────────────────────────────────────────────────────────────

void ReadSensors() {
    digitalWrite(SENSORPWR, HIGH);
    delay(50);

    // VIN — 10k/3k voltage divider: V_in = ADC * (5.0/1023) * (13.0/3.0)
    CurrentSenor[SENSOR_INPUT_VIN] = adcread(ADC_VIN) * (5.0f / 1023.0f) * (13.0f / 3.0f);

    float Voltage, RefVolt;
    int   Ref, maxVal, reading;

    // Input 1
    maxVal = 0;
    for (int i = 0; i < 20; i++) {
        reading = adcread(LEAKSENSOR1);
        if (reading > maxVal) maxVal = reading;
    }
    Ref     = adcread(VOLTSENSE);
    Voltage  = (maxVal / 1023.0f) * 5.0f;
    RefVolt  = (Ref    / 1023.0f) * 5.0f;
    CurrentSenor[SENSOR_INPUT_1] = abs((Voltage - RefVolt) / 0.07f) - 0.58f;
    if (CurrentSenor[SENSOR_INPUT_1] < 0) CurrentSenor[SENSOR_INPUT_1] = 0;

    // Input 2
    maxVal = 0;
    for (int i = 0; i < 20; i++) {
        reading = adcread(LEAKSENSOR2);
        if (reading > maxVal) maxVal = reading;
    }
    Ref     = adcread(VOLTSENSE);
    Voltage  = (maxVal / 1023.0f) * 5.0f;
    RefVolt  = (Ref    / 1023.0f) * 5.0f;
    CurrentSenor[SENSOR_INPUT_2] = abs((Voltage - RefVolt) / 0.07f) - 0.58f;
    if (CurrentSenor[SENSOR_INPUT_2] < 0) CurrentSenor[SENSOR_INPUT_2] = 0;

    // Input 3
    maxVal = 0;
    for (int i = 0; i < 20; i++) {
        reading = adcread(LEAKSENSOR3);
        if (reading > maxVal) maxVal = reading;
    }
    Ref     = adcread(VOLTSENSE);
    Voltage  = (maxVal / 1023.0f) * 5.0f;
    RefVolt  = (Ref    / 1023.0f) * 5.0f;
    CurrentSenor[SENSOR_INPUT_3] = abs((Voltage - RefVolt) / 0.07f) - 0.58f;
    if (CurrentSenor[SENSOR_INPUT_3] < 0) CurrentSenor[SENSOR_INPUT_3] = 0;

    digitalWrite(SENSORPWR, LOW);
}

float GetSensorValues(char Type) {
    switch (Type) {
        case SENSOR_INPUT_VIN: return CurrentSenor[SENSOR_INPUT_VIN];
        case SENSOR_INPUT_1:   return CurrentSenor[SENSOR_INPUT_1];
        case SENSOR_INPUT_2:   return CurrentSenor[SENSOR_INPUT_2];
        case SENSOR_INPUT_3:   return CurrentSenor[SENSOR_INPUT_3];
        default:               return 0;
    }
}

// ── Threshold setters ─────────────────────────────────────────────────────────

void SetSensorWaterDetect(unsigned int Value)   { SenorWaterFault           = Value; }
void SensorWaterDisconected(unsigned int Value) { SenorDisconectedThreshold = Value; }

// ── Misc ──────────────────────────────────────────────────────────────────────

char SensorValueCheck(double ValueIn) {
    if (ValueIn > SenorVoltageHighThreshold)                              return SENSORHIGHFAULT;
    if (ValueIn > SenorWaterFault && ValueIn < SenorDisconectedThreshold) return SENSOROK;
    return SENSORWATERDETECT;
}

char ReadRelayState() { return RelayState; }
