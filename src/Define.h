#ifndef DEFINE_H
#define DEFINE_H

// ── Digital IO pins ───────────────────────────────────────────────────────────
#define DIR       4
#define SENSORPWR 5
#define RELAY     6
#define LED       7

// ── ADC channel assignments ───────────────────────────────────────────────────
#define LEAKSENSOR1  0
#define LEAKSENSOR2  1
#define LEAKSENSOR3  2
#define VOLTSENSE    3
#define ADC_VIN      4   // 10k/3k voltage divider

// ── Sensor identity ───────────────────────────────────────────────────────────
#define CURRENTSENSORTYPE    30
#define CURRENTSENSORVERSION 2

// ── Holding register commands ─────────────────────────────────────────────────
#define SENSOR_CHANGE_ADDRESS    0x10
#define SENSOR_CHANGE_SCANRATE   0x11
#define SENSOR_CHANGE_FAULT      0x12
#define SENSOR_CHANGE_SENSOR_OPEN 0x13

// ── Coil indices ──────────────────────────────────────────────────────────────
#define SENSOR_SCAN_EN            0
#define SENSOR_LED_CONTROL        1
#define AUTO_SENSOR_RELAY_CONTROL 2
#define SENSOR_RELAY              3

// ── Timing ────────────────────────────────────────────────────────────────────
#define SENSORSCANRATE           2
#define SENSORTESTTIMEMULTIPLYER 1000

// ── Fault states ──────────────────────────────────────────────────────────────
#define SENSORHIGHFAULT   3
#define SENSORFAULT       2
#define SENSORWATERDETECT 1
#define SENSOROK          0

#endif
