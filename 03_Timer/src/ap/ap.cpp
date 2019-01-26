/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "ap.h"


void timerISR(void)
{
  ledToggle(_DEF_LED2);
}


void apInit(void)
{
  timerSetPeriod(_DEF_TIMER1, 500*1000);
  timerAttachInterrupt(_DEF_TIMER1, timerISR);
  timerStart(_DEF_TIMER1);
}

void apMain(void)
{
  while(1)
  {
    if (buttonGetPressed(_DEF_BUTTON1) == true)
    {
      ledOn(_DEF_LED1);
    }
    else
    {
      ledOff(_DEF_LED1);
    }
  }
}
