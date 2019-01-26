/*
 * timer.c
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */





#include "timer.h"



typedef struct
{
  TIM_HandleTypeDef    hTIM;
  TIM_OC_InitTypeDef   sConfig;
  uint32_t freq;
  void (*p_func)(void);
} hwtimer_t;




static hwtimer_t timer_tbl[TIMER_MAX_CH];



void timerInit(void)
{
  timer_tbl[_DEF_TIMER1].freq                        = 4000;
  timer_tbl[_DEF_TIMER1].hTIM.Instance               = TIM3;
  timer_tbl[_DEF_TIMER1].hTIM.Init.Prescaler         = (uint32_t)((SystemCoreClock / 2) / timer_tbl[_DEF_TIMER1].freq  ) - 1;   // 4Khz
  timer_tbl[_DEF_TIMER1].hTIM.Init.ClockDivision     = 0;
  timer_tbl[_DEF_TIMER1].hTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
  timer_tbl[_DEF_TIMER1].hTIM.Init.RepetitionCounter = 0;
  timer_tbl[_DEF_TIMER1].p_func = NULL;

  timerSetPeriod(_DEF_TIMER1, 1000);
}

void timerStop(uint8_t channel)
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[channel];

  HAL_TIM_Base_DeInit(&p_timer->hTIM);
}

void timerSetPeriod(uint8_t channel, uint32_t period_us)
{
  hwtimer_t *p_timer;
  uint32_t period;
  uint32_t time_div;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[channel];


  time_div = 1000000 / p_timer->freq;

  period = period_us/time_div;

  if (period == 0)
  {
    period = 1;
  }

  p_timer->hTIM.Init.Period = period - 1;
}

void timerAttachInterrupt(uint8_t channel, void (*func)(void))
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[channel];

  timerStop(channel);
  p_timer->p_func = func;
}

void timerDetachInterrupt(uint8_t channel)
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[channel];

  timerStop(channel);
  p_timer->p_func = NULL;
}

void timerStart(uint8_t channel)
{
  hwtimer_t *p_timer;

  if( channel >= TIMER_MAX_CH ) return;

  p_timer = &timer_tbl[channel];


  HAL_TIM_Base_Init(&p_timer->hTIM);
  HAL_TIM_Base_Start_IT(&p_timer->hTIM);
}

void timerCallback(TIM_HandleTypeDef *htim)
{
  uint8_t  i;
  hwtimer_t *p_timer;

  for (i=0; i<TIMER_MAX_CH; i++)
  {
    p_timer = &timer_tbl[i];

    if (htim->Instance == p_timer->hTIM.Instance)
    {
      if( p_timer->p_func != NULL )
      {
        (*p_timer->p_func)();
      }
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  timerCallback(htim);
}

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer_tbl[_DEF_TIMER1].hTIM);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3 )
  {
    __HAL_RCC_TIM3_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM3_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  }
}
