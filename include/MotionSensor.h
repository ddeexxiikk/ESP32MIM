#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include <Wire.h>

void setMotionSensor(uint8_t gpio = 5);
void setBuzzer(uint8_t gpio = 5, uint32_t timeout = 500);

#endif