#include "WRAPPER_HX711.h"
//instantiate the class
HX711 LoadCellAccel;                                         
HX711 LoadCellBrake;
HX711 LoadCellClutch;

#define MAX_SENSOR_OUTPUT_VAL 1023.0

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

long minAccelRead = 243383;     
long maxAccelRead = 1014050;
long minBrakeRead = 33419;
long maxBrakeRead = 363840;
long minClutchRead = 331367;
long maxClutchRead = 1175827;



float accelScalingFactor = 0.000944;
float brakeScalingFactor = 0.003389;
float clutchScalingFactor = 0.001352;

/*
Accel min val: 243383
Accel max val: 1014050
Accel scaling factor: 0.000130
Brake min val: 33419
Brake max val: 363840
Brake scaling factor: 0.000303
Clutch min val: 331367
Clutch max val: 1175827
Clutch scaling factor: 0.000118

*/

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
        scaledReading = MAX_SENSOR_OUTPUT_VAL;
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
      String IDEInput = Serial.readString(); // read the incoming data as string
      // Serial.print("Received String: ");
      // Serial.println(a);

      // start calibration window
      if (IDEInput == "calibrate"){
          calibrationStartTime = millis();
          calibrationUnderway = true;
          Serial.println("Calibration mode started"); 
          minAccelRead = accelReading;
          maxAccelRead = accelReading;  // resets min/max values if calibration is run again

          minBrakeRead = brakeReading;
          maxBrakeRead = brakeReading;

          minClutchRead = clutchReading;
          maxClutchRead = clutchReading;
      } else if(IDEInput == "print"){          
        printCalibrationData();
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

        accelScalingFactor = MAX_SENSOR_OUTPUT_VAL / (maxAccelRead - minAccelRead);    
        brakeScalingFactor = MAX_SENSOR_OUTPUT_VAL / (maxBrakeRead - minBrakeRead);     
        clutchScalingFactor = MAX_SENSOR_OUTPUT_VAL / (maxClutchRead - minClutchRead);
        printCalibrationData();

      }
  }
}

void returnAllScaledReadings(){
  accelReadingScaled = returnScaledReading(minAccelRead, maxAccelRead, accelReading, accelScalingFactor);
  brakeReadingScaled = returnScaledReading(minBrakeRead, maxBrakeRead, brakeReading, brakeScalingFactor);
  clutchReadingScaled = returnScaledReading(minClutchRead, maxClutchRead, clutchReading, clutchScalingFactor);
}

void printCalibrationData(){

    Serial.printf("\nAccel min val: %d\n", minAccelRead);
    Serial.printf("Accel max val: %d\n", maxAccelRead);
    Serial.printf("Accel scaling factor: %f\n", accelScalingFactor);
   
    Serial.printf("Brake min val: %d\n", minBrakeRead);
    Serial.printf("Brake max val: %d\n", maxBrakeRead);
    Serial.printf("Brake scaling factor: %f\n", brakeScalingFactor);

    Serial.printf("Clutch min val: %d\n", minClutchRead);
    Serial.printf("Clutch max val: %d\n", maxClutchRead);
    Serial.printf("Clutch scaling factor: %f\n\n", clutchScalingFactor);
}