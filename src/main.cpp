#include "includes.h"

float accelReadingScaled;
float brakeReadingScaled;
float clutchReadingScaled;

void setup() {

  Serial.begin(115200);
  init_HX711(DOUT_PIN_ACCEL, SCK_PIN_ACCEL, DOUT_PIN_BRAKE, SCK_PIN_BRAKE, DOUT_PIN_CLUTCH, SCK_PIN_CLUTCH);

}

void loop() {

  getSensorValues();
  calibrate();
  returnScaledReadings();
  
  // Serial.printf("Accel: %d Brake: %d Clutch: %d \n", accelReading, brakeReading, clutchReading);
  delay(300);
  Serial.printf("Accel: %f   Brake: %f   Clutch: %f\n", accelReadingScaled, brakeReadingScaled, clutchReadingScaled);
  


  

}