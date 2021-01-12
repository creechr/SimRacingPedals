#ifndef CONFIG_H
#define CONFIG_H


#define DOUT_PIN_ACCEL 1
#define SCK_PIN_ACCEL 2

#define DOUT_PIN_BRAKE 10
#define SCK_PIN_BRAKE 11

#define DOUT_PIN_CLUTCH 29
#define SCK_PIN_CLUTCH 30

long accelReading = 0;
long accelReadingLast = 0;

long brakeReading = 0;
long brakeReadingLast = 0;

long clutchReading = 0;
long clutchReadingLast = 0;

#endif