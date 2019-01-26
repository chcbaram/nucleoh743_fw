/*
 * led.c
 *
 *  Created on: 2019. 1. 22.
 *      Author: Baram
 */




#include "led.h"





void ledInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void ledOn(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
      break;
  }
}

void ledOff(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
      break;
  }
}

void ledToggle(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
      break;
  }
}
