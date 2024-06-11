#include "Buzzer.h"

//d4

void setupBuzzer(uint8_t gpio)
{
    pinMode(gpio, OUTPUT);
}

void setBuzzer(uint8_t gpio)
{
    digitalWrite(gpio, !digitalRead(gpio));
}