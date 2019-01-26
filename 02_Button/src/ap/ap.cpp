/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "ap.h"





void apInit(void)
{
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
