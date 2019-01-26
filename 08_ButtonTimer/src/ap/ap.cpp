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
  uint32_t button_cnt = 0;
  uint32_t cur_cnt = 0;


  while(1)
  {
    if (buttonGetPressed(_DEF_BUTTON1))
    {
      cur_cnt = buttonGetPressedTime(_DEF_BUTTON1)/1000;
      if (cur_cnt > button_cnt)
      {
        button_cnt++;
        uartPrintf(_DEF_UART1, "Button Cnt : %d\n", button_cnt);
      }
    }
  }
}
