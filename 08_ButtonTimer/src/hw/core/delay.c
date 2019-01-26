/*
 * delay.c
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "delay.h"
#include "micros.h"



void delayInit(void)
{

}

void delayNs(uint32_t ns)
{
  for (volatile uint32_t i = 0; i < ns/10; i++) { }
}

void delayUs(uint32_t us)
{
  uint32_t t_time;


  t_time = micros();

  while(1)
  {
    if ((micros()-t_time) >= us)
    {
      break;
    }
  }
}

void delayMs(uint32_t ms)
{
  HAL_Delay(ms);
}
