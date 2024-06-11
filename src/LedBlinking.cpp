#include "LedBlinking.h"
#include "globals.h"

void setupLed(uint8_t gpio)
{
    pinMode(gpio, OUTPUT);
}

void blinkLed(uint8_t gpio)
{
    if (isOkay == 0)
        digitalWrite(gpio, HIGH);
    else
        digitalWrite(gpio, LOW);
}