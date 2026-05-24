#ifndef SENSOR_H
#define SENSOR_H

// ── Sensor value type indices (argument to GetSensorValues) ───────────────────
#define SENSOR_INPUT_VIN 0
#define SENSOR_INPUT_1   1
#define SENSOR_INPUT_2   2
#define SENSOR_INPUT_3   3

// ── Init ──────────────────────────────────────────────────────────────────────
void SensorStart();
void adcinit();

// ── Acquisition ───────────────────────────────────────────────────────────────
void  ReadSensors();
float GetSensorValues(char Type);
int   adcread(unsigned char channel);

// ── Threshold setters ─────────────────────────────────────────────────────────
void SetSensorWaterDetect(unsigned int Value);
void SensorWaterDisconected(unsigned int Value);

// ── Misc ──────────────────────────────────────────────────────────────────────
char  SensorValueCheck(double ValueIn);
char  ReadRelayState();

#endif
