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
    delayUs(500*1000);
  }
}
