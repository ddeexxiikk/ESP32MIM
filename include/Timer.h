#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

void setupTimer();
void IRAM_ATTR onTimer();

#endif