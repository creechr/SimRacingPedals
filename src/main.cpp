#include "includes.h"


void setup() {

  Serial.begin(115200);
  init_HX711(DOUT_PIN_ACCEL, SCK_PIN_ACCEL, DOUT_PIN_BRAKE, SCK_PIN_BRAKE, DOUT_PIN_CLUTCH, SCK_PIN_CLUTCH);

}

void loop() {

  accelReading = readAccel(accelReadingLast);
  brakeReading = readBrake(brakeReadingLast);
  clutchReading = readClutch(clutchReadingLast);
    

  //if (accelReadingLast != accelReading) {
  //  Serial.println(millis());
  //}
  
  accelReadingLast = accelReading;
  brakeReadingLast = brakeReading;
  clutchReadingLast = clutchReading;

  
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

accelReadingScaled = returnScaledReading(minAccelRead, maxAccelRead, accelReading, accelScalingFactor);
brakeReadingScaled = returnScaledReading(minBrakeRead, maxBrakeRead, brakeReading, brakeScalingFactor);
clutchReadingScaled = returnScaledReading(minClutchRead, maxClutchRead, clutchReading, clutchScalingFactor);

// Serial.printf("Accel: %d Brake: %d Clutch: %d \n", accelReading, brakeReading, clutchReading);
delay(300);
Serial.printf("Accel: %f   Brake: %f   Clutch: %f\n", accelReadingScaled, brakeReadingScaled, clutchReadingScaled);
  


  

}