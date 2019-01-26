/*
 * led.c
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "led.h"


typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;


led_tbl_t led_tbl[LED_MAX_CH] =
{
  {GPIOB, GPIO_PIN_0,  GPIO_PIN_SET, GPIO_PIN_RESET},
  {GPIOB, GPIO_PIN_7,  GPIO_PIN_SET, GPIO_PIN_RESET},
  {GPIOB, GPIO_PIN_14, GPIO_PIN_SET, GPIO_PIN_RESET},
};


void ledInit(void)
{
  uint32_t i;
  GPIO_InitTypeDef GPIO_InitStruct;


  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for (i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = led_tbl[i].pin;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStruct);

    ledOff(i);
  }
}

void ledOn(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
  }
}

void ledOff(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
  }
}

void ledToggle(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
  }
}
