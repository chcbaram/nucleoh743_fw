/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 22.
 *      Author: Baram
 */




#include "ap.h"


extern UART_HandleTypeDef huart3;


void apInit(void)
{
}

void apMain(void)
{
  while(1)
  {
    ledToggle(0);
    delay(500);
    HAL_UART_Transmit(&huart3, (uint8_t *)"Test\r\n", 6, 100);
  }
}
