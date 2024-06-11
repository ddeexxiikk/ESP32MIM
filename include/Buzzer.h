#ifndef BUZZER_H
#define BUZZER_H

#include <Wire.h>

void setupBuzzer(uint8_t gpio = 4);
void setBuzzer(uint8_t gpio = 4, int isOkay = 1);

#endif
