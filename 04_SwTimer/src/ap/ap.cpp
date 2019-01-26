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

void swtimerISR(void *arg)
{
  ledToggle(_DEF_LED3);
}

void apInit(void)
{
  swtimer_handle_t h_led_timer;


  timerSetPeriod(_DEF_TIMER1, 500*1000);
  timerAttachInterrupt(_DEF_TIMER1, timerISR);
  timerStart(_DEF_TIMER1);

  h_led_timer = swtimerGetHandle();
  swtimerSet(h_led_timer, 500, LOOP_TIME, swtimerISR, NULL );
  swtimerStart(h_led_timer);

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
