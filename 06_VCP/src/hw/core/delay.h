/*
 * delay.h
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_DELAY_H_
#define SRC_HW_CORE_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


void delayInit();

void delayNs(uint32_t ns);
void delayUs(uint32_t us);
void delayMs(uint32_t ms);


#ifdef __cplusplus
}
#endif


#endif /* SRC_HW_CORE_DELAY_H_ */
