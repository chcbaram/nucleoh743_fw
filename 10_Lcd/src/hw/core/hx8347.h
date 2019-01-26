/*
 *  st7735.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef ST7735_H
#define ST7735_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "hw_def.h"





#define HX8347D_BLACK   0x0000
#define HX8347D_BLUE    0x001F
#define HX8347D_RED     0xF800
#define HX8347D_GREEN   0x07E0
#define HX8347D_CYAN    0x07FF
#define HX8347D_MAGENTA 0xF81F
#define HX8347D_YELLOW  0xFFE0
#define HX8347D_WHITE   0xFFFF


// Color definitions
#define _BLACK          HX8347D_BLACK
#define _BLUE           HX8347D_BLUE
#define _RED            HX8347D_RED
#define _GREEN          HX8347D_GREEN
#define _CYAN           HX8347D_CYAN
#define _MAGENTA        HX8347D_MAGENTA
#define _YELLO          HX8347D_YELLOW
#define _WHITE          HX8347D_WHITE



void hx8347Init();

void hx8347FillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color);
void hx8347DrawFrame(bool wait);
void hx8347SetRotation(uint8_t mode);

uint16_t hx8347GetWidth(void);
uint16_t hx8347GetHeight(void);

void hx8347DrawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w);
void hx8347DrawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h);
void hx8347SendBuffer(uint16_t *buffer, uint16_t n);
void hx8347SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void hx8347DrawPixel(int16_t x, int16_t y, uint16_t color);
bool hx8347IsBusy(void);



#ifdef __cplusplus
}
#endif


#endif
