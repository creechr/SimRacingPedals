#include "WRAPPER_HX711.h"
//instantiate the class
HX711 LoadCellAccel;                                         
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

long accelReading = 0;
long accelReadingLast = 0;

long brakeReading = 0;
long brakeReadingLast = 0;

long clutchReading = 0;
long clutchReadingLast = 0;

unsigned long timeNow = 0;
unsigned long calibrationStartTime = 0;
unsigned long calibrationPeriod = 10000; 
bool calibrationUnderway = false;

long minAccelRead;     
long maxAccelRead;
long minBrakeRead;
long maxBrakeRead;
long minClutchRead;
long maxClutchRead;

float maxSensorOutputVal = 100.0;

float accelScalingFactor;
float brakeScalingFactor;
float clutchScalingFactor;



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

void getSensorValues(){
  accelReading = readAccel(accelReadingLast);
  brakeReading = readBrake(brakeReadingLast);
  clutchReading = readClutch(clutchReadingLast);

  accelReadingLast = accelReading;
  brakeReadingLast = brakeReading;
  clutchReadingLast = clutchReading;
}

void calibrate(){
  if (Serial.available() > 0) {
      String a = Serial.readString(); // read the incoming data as string
      // Serial.print("Received String: ");
      // Serial.println(a);

      // start calibration window
      if (a == "calibrate")
      {
          calibrationStartTime = millis();
          calibrationUnderway = true;
          Serial.println("Calibration mode started"); 
          minAccelRead = accelReading;
          maxAccelRead = accelReading;  // resets min/max values if calibration is run again

          minBrakeRead = brakeReading;
          maxBrakeRead = brakeReading;

          minClutchRead = clutchReading;
          maxClutchRead = clutchReading;
      }

  }

  timeNow = millis();

  if (calibrationUnderway){
      if (timeNow < (calibrationStartTime + calibrationPeriod)){
          // Serial.println("Calibration in progress");

          minAccelRead = min(accelReading, minAccelRead);
          maxAccelRead = max(accelReading, maxAccelRead);

          minBrakeRead = min(brakeReading, minBrakeRead);
          maxBrakeRead = max(brakeReading, maxBrakeRead);

          minClutchRead = min(clutchReading, minClutchRead);
          maxClutchRead = max(clutchReading, maxClutchRead);
    
      } else {
        calibrationUnderway = false;
        Serial.println("Calibration finished");

        accelScalingFactor = maxSensorOutputVal / (maxAccelRead - minAccelRead);        
        Serial.printf("Accel min val: %d\n", minAccelRead);
        Serial.printf("Accel max val: %d\n", maxAccelRead);
        Serial.printf("Accel scaling factor: %f\n", accelScalingFactor);

        brakeScalingFactor = maxSensorOutputVal / (maxBrakeRead - minBrakeRead);
        Serial.printf("Brake min val: %d\n", minBrakeRead);
        Serial.printf("Brake max val: %d\n", maxBrakeRead);
        Serial.printf("Brake scaling factor: %f\n", brakeScalingFactor);

        clutchScalingFactor = maxSensorOutputVal / (maxClutchRead - minClutchRead);
        Serial.printf("Clutch min val: %d\n", minClutchRead);
        Serial.printf("Clutch max val: %d\n", maxClutchRead);
        Serial.printf("Clutch scaling factor: %f\n", clutchScalingFactor);

      }
  }
}

void returnScaledReadings(){
  accelReadingScaled = returnScaledReading(minAccelRead, maxAccelRead, accelReading, accelScalingFactor);
  brakeReadingScaled = returnScaledReading(minBrakeRead, maxBrakeRead, brakeReading, brakeScalingFactor);
  clutchReadingScaled = returnScaledReading(minClutchRead, maxClutchRead, clutchReading, clutchScalingFactor);
}
