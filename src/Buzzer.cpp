#include "Buzzer.h"
#include "globals.h"

void setupBuzzer(uint8_t gpio)
{
    pinMode(gpio, OUTPUT);
    digitalWrite(gpio, HIGH);
}

void setBuzzer(uint8_t gpio)
{
     if (isOkay == 0)
        digitalWrite(gpio, LOW); // Włącz buzzer, gdy isOkay wynosi 0
    else
        digitalWrite(gpio, HIGH); // Wyłącz buzzer, gdy isOkay wynosi 1
}