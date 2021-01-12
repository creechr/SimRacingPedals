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
            lastAccelReading = LoadCellAccel.read();
            //Serial.print("HX711 reading: ");
            //Serial.println(lastAccelReading);

            readingRetryFlag1 = 1;
            flagTimerStart1 = millis();
        } else {
            return lastAccelReading;
            //Serial.println("HX711 not found.");
            //delay(1000);
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
            //Serial.print("HX711 reading: ");
            //Serial.println(lastAccelReading);

            readingRetryFlag2 = 1;
            flagTimerStart2 = millis();
        } else {
            return lastBrakeReading;
            //Serial.println("HX711 not found.");
            //delay(1000);
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
            lastClutchReading = LoadCellClutch.read();
            //Serial.print("HX711 reading: ");
            //Serial.println(lastAccelReading);

            readingRetryFlag3 = 1;
            flagTimerStart3 = millis();
        } else {
            return lastClutchReading;
            //Serial.println("HX711 not found.");
            //delay(1000);
        }
    }
    return lastClutchReading;
} 