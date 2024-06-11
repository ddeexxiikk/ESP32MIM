#include "MotionSensor.h"
#include <Wire.h>
#include <Arduino.h>

void setMotionSensor(uint8_t gpio)
{
  pinMode(gpio, INPUT);
}

void setBuzzer(uint8_t gpio, uint32_t timeout)
{
  pinMode(gpio, OUTPUT);
  digitalWrite(gpio, LOW);
  delay(timeout);
  digitalWrite(gpio, HIGH);
}