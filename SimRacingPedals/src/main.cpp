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

  // Serial.println(accelReading);
  Serial.printf("Accel: %d Brake: %d Clutch: %d \n", accelReading, brakeReading, clutchReading);
  // Serial.println(brakeReading);
  // Serial.println(clutchReading);
  
  

}