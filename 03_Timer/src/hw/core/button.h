/*
 * button.h
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_BUTTON_H_
#define SRC_HW_CORE_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#define BUTTON_MAX_CH       1



void buttonInit(void);
bool buttonGetPressed(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_BUTTON_H_ */
