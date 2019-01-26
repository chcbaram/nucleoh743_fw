/*
 *  lcd.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef LCD_H
#define LCD_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "hw_def.h"



typedef struct
{

} lcd_drv_t;




void lcdInit();

void lcdDrawFrame(bool wait);
void lcdSetRotation(uint8_t mode);
void lcdFillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color);
void lcdFillScreen(uint16_t color);
void lcdDrawPixel(int16_t x, int16_t y, uint16_t color);
bool lcdIsBusy(void);
void lcdPrintf(int16_t x, int16_t y, const char *fmt, ...);
uint16_t lcdWidth(void);
uint16_t lcdHeight(void);


#ifdef __cplusplus
}
#endif


#endif
