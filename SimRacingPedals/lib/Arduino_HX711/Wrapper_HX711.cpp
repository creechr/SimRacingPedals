#include "WRAPPER_HX711.h"
HX711 LoadCellAccel;                                            //instantiate the class

uint8_t readingRetryFlag;
uint16_t flagTimer;
uint16_t flagTimerStart;
uint16_t delayTime;

void init_HX711(uint8_t dOutPin, uint8_t sckPin) {
    LoadCellAccel.begin(dOutPin, sckPin);

    readingRetryFlag = 0;                                              // is set to 1 if LoadCellAccel.is_ready() returns false and is set to 0 when timer is passed
    flagTimer = 0;
    flagTimerStart = millis();
    delayTime = 20;                                                     // in ms
}

long readLoadCell(long lastReading) {

    flagTimer = millis();

    if ((flagTimer-flagTimerStart) > delayTime) {
        readingRetryFlag = 0;
    }


    if (readingRetryFlag == 0) {
        if (LoadCellAccel.is_ready()) {
            lastReading = LoadCellAccel.read();
            //Serial.print("HX711 reading: ");
            //Serial.println(lastReading);

            readingRetryFlag = 1;
            flagTimerStart = millis();
        } else {
            return lastReading;
            //Serial.println("HX711 not found.");
            //delay(1000);
        }
    }
    return lastReading;
}