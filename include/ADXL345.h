#ifndef ADXL345_H
#define ADXL345_H

#include <Adafruit_Sensor.h>

void displaySensorDetails();
void displayDataRate();
void displayRange();
void setupSensor();
sensors_event_t readSensorX();
sensors_event_t readSensorY();
sensors_event_t readSensorZ();

void handleInterrupt();
#endif