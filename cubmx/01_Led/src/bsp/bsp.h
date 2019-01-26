/*
 * bsp.h
 *
 *  Created on: 2019. 1. 22.
 *      Author: Baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"
#include "stm32h7xx_hal.h"



void bspInit(void);


void delay(uint32_t delay_ms);
uint32_t millis(void);
uint32_t micros(void);


#ifdef __cplusplus
 }
#endif

#endif /* SRC_BSP_BSP_H_ */
