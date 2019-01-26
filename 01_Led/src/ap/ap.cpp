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
    ledToggle(_DEF_LED1);
    ledToggle(_DEF_LED2);
    ledToggle(_DEF_LED3);
    delay(500);
  }
}
