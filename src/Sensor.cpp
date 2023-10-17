#include "Sensor.h"
#include <Arduino.h>

#include "Define.h"

//These values can be modified 
unsigned int SenorWaterFault = 550;
unsigned int SenorDisconectedThreshold = 600;

//This is hard coded, board Level issue
int SenorVoltageHighThreshold = 800;
int SenorVoltageLowThreshold = 270;
float Vin = 0;
int VinAdc[VOLT_AVG];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
long Vintotal = 0;     

char LeakSenor[5] = {SENSOROK,SENSOROK,SENSOROK,SENSOROK};

char RelayState = 0;

char ReadSensors(){
  digitalWrite(SENSORPWR, HIGH);
  delay(50);
  //float VoltSense = analogRead(VOLTSENSE)/1025.0*5*2;
  //Serial.print(" SensorVoltage = ");
  //Serial.println(VoltSense);
/*     if((VoltSense >= SenorVoltageLowThreshold/100) and (VoltSense <= SenorVoltageHighThreshold/100)){
    LeakSenor[0] = SENSOROK;
  }
  else{
    LeakSenor[0] = SENSORFAULT;
    digitalWrite(SENSORPWR, LOW);
    return 0;
  } */
  double SensorA = 0;
  int NextSample = 0;
  int LastSample = analogRead(LEAKSENSOR1);
  int Ref = 0;
  int max = 0, reading;
  for(int i = 0; i<20; i++) {
    reading = analogRead(LEAKSENSOR1); // change number for pin you are using
    if(reading > max){
      max = reading;
    }
    Ref = analogRead(VOLTSENSE);
  }
  float Voltage = (max/1023.0)*5.0;
  float RefVolt = (Ref/1023.0)*5.0;
  float Current = abs(Voltage-RefVolt);
  //SensorB = SensorB/5;
  //SensorC = SensorC/5;
  //Serial.print(SensorA);
  //Serial.print(" ");
  ///Serial.print(SensorB);
  //Serial.print(" ");
  Serial.println(Current);
  //digitalWrite(SENSORPWR, LOW);
  return 0;
}

void SampleVin(){
  Vintotal = Vintotal - VinAdc[readIndex];
  // read from the sensor:
  VinAdc[readIndex] = analogRead(VIN);
  // add the reading to the total:
  Vintotal = Vintotal + VinAdc[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= VOLT_AVG) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  Vin = (Vintotal / VOLT_AVG)*(5.1/1024)*5.6;
  //Serial.print("Vin = ");
  //Serial.println(Vin);
}

char SensorValueCheck(double ValueIn){
  if(ValueIn > SenorVoltageHighThreshold){
    return SENSORHIGHFAULT;
  }
  else if((ValueIn > SenorWaterFault) and (ValueIn < SenorDisconectedThreshold)){
    return SENSOROK;
  }
  else{
    return SENSORWATERDETECT;
  }
}

char RelayCheck(){
  char error = 0;
  unsigned char k;
  for( k= 0;k<5;k++){
    if(LeakSenor[k] != SENSOROK){
      error++;
    }
  }
  if(error){
    digitalWrite(RELAY, HIGH);
    RelayState = 1;
    return  0;
  }
  else{
    digitalWrite(RELAY, LOW);
    RelayState = 0;
    return  1;
  }
}

char ReadSensorValues(unsigned char Value){
    return LeakSenor[Value];
}

char ReadRelayState(){
    return RelayState;
}

void SetSensorWaterDetect(unsigned int Value){
    SenorWaterFault = Value;
}

void SensorWaterDisconected(unsigned int Value){
    SenorDisconectedThreshold = Value;
}

float GetSensorValues(char Sensor){
  return Vin;
}
