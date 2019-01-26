/*
 *  lcd.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "lcd.h"

#include "cmdif.h"
#include "hx8347.h"
#include "hangul/PHan_Lib.h"



static uint16_t text_color = _WHITE;
static uint16_t text_bg_color = _BLACK;


void lcdCmdif(void);




void lcdInit(void)
{

  hx8347Init();


  lcdSetRotation(0);


  lcdFillScreen(_BLACK);
  lcdPrintf(0, 0, "LCD 테스트 ");
  lcdDrawFrame(true);

  cmdifAdd("lcd", lcdCmdif);
}

void lcdFillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color)
{
  hx8347FillRect(x, y, w, h, color);
}

void lcdFillScreen(uint16_t color)
{
  hx8347FillRect(0, 0,  hx8347GetWidth(), hx8347GetHeight(), color);
}

void lcdDrawFrame(bool wait)
{
  hx8347DrawFrame(wait);
}

bool lcdIsBusy(void)
{
  return hx8347IsBusy();
}

void lcdSetRotation(uint8_t mode)
{
  hx8347SetRotation(mode);
}

uint16_t lcdWidth(void)
{
  return hx8347GetWidth();
}

uint16_t lcdHeight(void)
{
  return hx8347GetHeight();
}

void lcdDrawPixel(int16_t x, int16_t y, uint16_t color)
{
  hx8347DrawPixel(x, y, color);
}

void lcdDisHanFont(int x, int y, PHAN_FONT_OBJ *FontPtr)
{
  uint16_t  i, j, Loop;
  uint16_t  FontSize = FontPtr->Size_Char;
  uint16_t index_x;

  if (FontSize > 2)
  {
    FontSize = 2;
  }

  for ( i = 0 ; i < 16 ; i++ )        // 16 Lines per Font/Char
  {
    index_x = 0;
    for ( j = 0 ; j < FontSize ; j++ )      // 16 x 16 (2 Bytes)
    {
      for( Loop=0; Loop<8; Loop++ )
      {
        if( FontPtr->FontBuffer[i*FontSize +j] & (0x80>>Loop))
        {
          lcdDrawPixel(x + index_x++, y + i, text_color);
        }
        else
        {
          lcdDrawPixel(x + index_x++, y + i, text_bg_color);
        }
      }
    }
  }
}

void lcdPrintf(int16_t x, int16_t y, const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];
  int Size_Char;
  int i, x_Pre = x;
  PHAN_FONT_OBJ FontBuf;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  for( i=0; i<len; i+=Size_Char )
  {
    PHan_FontLoad( &print_buffer[i], &FontBuf );


    lcdDisHanFont( x, y, &FontBuf );

    Size_Char = FontBuf.Size_Char;
    if (Size_Char >= 2)
    {
        x += 2*8;
    }
    else
    {
        x += 1*8;
    }

    if( lcdWidth() < x )
    {
        x  = x_Pre;
        y += 16;
    }

    if( FontBuf.Code_Type == PHAN_END_CODE ) break;
  }
}


//-- lcdCmdif
//
void lcdCmdif(void)
{
  bool ret = true;
  uint32_t pre_time;
  uint32_t x_pos = 0;
  uint32_t y_pos = 100;

  if (cmdifHasString("test", 0) == true)
  {
    while(cmdifRxAvailable() == 0)
    {

      pre_time = millis();
      lcdFillRect(x_pos, y_pos, 50, 50, _BLACK);

      x_pos += 10;
      x_pos %= lcdWidth()-50;

      lcdFillRect(x_pos, y_pos, 50, 50, _BLUE);
      lcdDrawFrame(true);
      lcdPrintf(0, 0, "LCD 그리기 시간 : %d ms", millis()-pre_time);
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cmdifPrintf( "lcd test\n");
  }
}


