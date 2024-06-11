#ifndef LEDBLINKING_H
#define LEDBLINKING_H

#include <Wire.h>

void setupLed(uint8_t gpio = 2);
void blinkLed(uint8_t gpio = 2);

#endif