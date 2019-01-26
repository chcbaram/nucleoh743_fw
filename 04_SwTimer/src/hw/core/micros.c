/*
 * micros.c
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "micros.h"



static TIM_HandleTypeDef  TimHandle;



void microsInit(void)
{
  __HAL_RCC_TIM2_CLK_ENABLE();


  /* Set TIMx instance */
  TimHandle.Instance = TIM2;

  /* Initialize TIM3 peripheral as follow:
         + Period = 10000 - 1
         + Prescaler = ((SystemCoreClock/2)/10000) - 1
         + ClockDivision = 0
         + Counter direction = Up
   */
  TimHandle.Init.Period         = 0xFFFFFFFF;
  TimHandle.Init.Prescaler      = (SystemCoreClock/2/1000000)-1;
  TimHandle.Init.ClockDivision  = 0;
  TimHandle.Init.CounterMode    = TIM_COUNTERMODE_UP;

  HAL_TIM_Base_Init(&TimHandle);
  HAL_TIM_Base_Start(&TimHandle);
}

uint32_t micros(void)
{
  return TimHandle.Instance->CNT;
}
