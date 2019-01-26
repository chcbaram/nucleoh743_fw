/*
 * led.h
 *
 *  Created on: 2019. 1. 22.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_LED_H_
#define SRC_HW_CORE_LED_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"



void ledInit(void);

void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);


#ifdef __cplusplus
 }
#endif

#endif /* SRC_HW_CORE_LED_H_ */
