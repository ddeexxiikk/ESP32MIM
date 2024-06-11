#include "LedBlinking.h"

void setupLed(uint8_t gpio)
{
    pinMode(gpio, OUTPUT);
}

void blinkLed(uint8_t gpio)
{
    digitalWrite(gpio, !digitalRead(gpio));
}

void turnOffLed(uint8_t gpio)
{
    digitalWrite(gpio, LOW);
}