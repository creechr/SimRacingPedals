/*

meaningful information

*/


#ifndef WRAPPER_HX711_H
#define WRAPPER_HX711_H


#include <Arduino.h>
#include "HX711.h"


extern float accelReadingScaled;
extern float brakeReadingScaled;
extern float clutchReadingScaled;



void init_HX711(uint8_t dOutPinAccel, uint8_t sckPinAccel, uint8_t dOutPinBrake, uint8_t sckPinBrake, uint8_t dOutPinClutch, uint8_t sckPinClutch);
long readAccel(long lastAccelReading);
long readBrake(long lastBrakeReading);
long readClutch(long lastClutchReading);
float returnScaledReading(long minValue, long maxValue, long currentValue, float scalingFactor);
void getSensorValues();
void calibrate();
void returnAllScaledReadings();
void printCalibrationData();

#endif