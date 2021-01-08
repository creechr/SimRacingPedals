/*

meaningful information

*/


#ifndef WRAPPER_HX711_H
#define WRAPPER_HX711_H


#include <Arduino.h>
#include "HX711.h"



void init_HX711(uint8_t dOutPin, uint8_t sckPin);
long readLoadCell(long lastReading);


#endif