#include "Timer.h"
#include "LedBlinking.h"
#include "Buzzer.h"
#include "ADXL345.h"

hw_timer_t* timer = NULL;

void setupTimer()
{
    timer = timerBegin(0, 2400, true);
    timerAlarmWrite(timer, 20000, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmEnable(timer);
}

void IRAM_ATTR onTimer()
{
    blinkLed();
    setBuzzer();
}