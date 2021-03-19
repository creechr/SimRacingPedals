#include "WRAPPER_HX711.h"
HX711 LoadCellAccel;                                            //instantiate the class
HX711 LoadCellBrake;
HX711 LoadCellClutch;


uint8_t readingRetryFlag1;
uint16_t flagTimer1;
uint16_t flagTimerStart1;
uint16_t delayTime;

uint8_t readingRetryFlag2;
uint16_t flagTimer2;
uint16_t flagTimerStart2;


uint8_t readingRetryFlag3;
uint16_t flagTimer3;
uint16_t flagTimerStart3;

float scaledReading;

void init_HX711(uint8_t dOutPinAccel, uint8_t sckPinAccel, uint8_t dOutPinBrake, uint8_t sckPinBrake, uint8_t dOutPinClutch, uint8_t sckPinClutch) {
    LoadCellAccel.begin(dOutPinAccel, sckPinAccel);
    LoadCellBrake.begin(dOutPinBrake, sckPinBrake);
    LoadCellClutch.begin(dOutPinClutch, sckPinClutch);

    readingRetryFlag1 = 0;                                              // is set to 1 if LoadCellAccel.is_ready() returns false and is set to 0 when timer is passed
    flagTimer1 = 0;
    flagTimerStart1 = millis();
    readingRetryFlag2 = 0;
    flagTimer2 = 0;
    flagTimerStart2 = millis();
    readingRetryFlag3 = 0;
    flagTimer3 = 0;
    flagTimerStart3 = millis();
    delayTime = 20;                                                     // in ms
}

long readAccel(long lastAccelReading) {

    flagTimer1 = millis();

    if ((flagTimer1-flagTimerStart1) > delayTime) {
        readingRetryFlag1 = 0;
    }
    if (readingRetryFlag1 == 0) {
        if (LoadCellAccel.is_ready()) {
            lastAccelReading = abs(LoadCellAccel.read());

            readingRetryFlag1 = 1;
            flagTimerStart1 = millis();
        } else {
            return lastAccelReading;
            
        }
    }
    return lastAccelReading;
} 

long readBrake(long lastBrakeReading) {

    flagTimer2 = millis();

    if ((flagTimer2-flagTimerStart2) > delayTime) {
        readingRetryFlag2 = 0;
    }
    if (readingRetryFlag2 == 0) {
        if (LoadCellBrake.is_ready()) {
            lastBrakeReading = LoadCellBrake.read();
          

            readingRetryFlag2 = 1;
            flagTimerStart2 = millis();
        } else {
            return lastBrakeReading;
            
        }
    }
    return lastBrakeReading;
} 

long readClutch(long lastClutchReading) {

    flagTimer3 = millis();

    if ((flagTimer3-flagTimerStart3) > delayTime) {
        readingRetryFlag3 = 0;
    }
    if (readingRetryFlag3 == 0) {
        if (LoadCellClutch.is_ready()) {
            lastClutchReading = abs(LoadCellClutch.read());
      

            readingRetryFlag3 = 1;
            flagTimerStart3 = millis();
        } else {
            return lastClutchReading;
            
        }
    }
    return lastClutchReading;
} 


// pass min, max, current & scaling factor then output a scaled value with a max of "maxSensorOutputVal"

float returnScaledReading(long minValue, long maxValue, long currentValue, float scalingFactor){

    // Serial.printf("minValue: %d maxValue: %d currentValue: %d scaling factor: %f\n", minValue, maxValue, currentValue, scalingFactor);    
    
    if (currentValue > maxValue){
        scaledReading = 100.0;
    } else if (currentValue < minValue){
        scaledReading = 0;
    } else {
        scaledReading = (currentValue - minValue) * scalingFactor;
    }
    // Serial.printf("Scaled reading: %f\n", scaledReading);
    return scaledReading; 
}