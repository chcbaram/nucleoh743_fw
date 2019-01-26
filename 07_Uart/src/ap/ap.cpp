/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "ap.h"



void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);
}

void apMain(void)
{
  while(1)
  {
    if (uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "uart ch1 rx : 0x%X\n", uartRead(_DEF_UART1));
    }
  }
}
