/*
 * hw.c
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "hw.h"





void hwInit(void)
{
  bspInit();

  swtimerInit();
  ledInit();
  buttonInit();
  microsInit();
  timerInit();
  delayInit();
}
