#include "includes.h"


void setup() {

  Serial.begin(115200);
  init_HX711(DOUT_PIN_ACCEL, SCK_PIN_ACCEL);


}

void loop() {

  accelReading = readLoadCell(accelReadingLast);
  //if (accelReadingLast != accelReading) {
  //  Serial.println(millis());
  //}
  accelReadingLast = accelReading;

  Serial.println(accelReading);
  

}