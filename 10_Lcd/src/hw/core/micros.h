/*
 * micros.h
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */

#ifndef SRC_HW_CORE_MICROS_H_
#define SRC_HW_CORE_MICROS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


void microsInit(void);
uint32_t micros(void);


#ifdef __cplusplus
}
#endif


#endif /* SRC_HW_CORE_MICROS_H_ */
