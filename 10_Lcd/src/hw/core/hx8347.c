/*
 *  hx8347.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "hx8347.h"
#include "spi.h"



#define _TFTWIDTH       320
#define _TFTHEIGHT      240



#define _PIN_DEF_DC     0
#define _PIN_DEF_CS     1
#define _PIN_DEF_RST    2
#define _PIN_DEF_BL     3
#define _PIN_DEF_SD     4



uint32_t colstart = 0;
uint32_t rowstart = 0;


typedef struct
{
  GPIO_TypeDef *p_port;
  uint16_t      pin_num;
} gpio_pin_t;





static gpio_pin_t pin_tbl[5] =
  {
  {GPIOF, GPIO_PIN_13},  // _PIN_DEF_DC
  {GPIOD, GPIO_PIN_14},  // _PIN_DEF_CS
  {GPIOF, GPIO_PIN_12},  // _PIN_DEF_RST
  {GPIOD, GPIO_PIN_15},  // _PIN_DEF_BL
  {GPIOE, GPIO_PIN_11},  // _PIN_DEF_SD
  };


static uint32_t _width  = _TFTWIDTH;
static uint32_t _height = _TFTHEIGHT;



static uint16_t frame_buf[_TFTWIDTH*_TFTWIDTH]  __attribute__((section(".NoneCacheableMem")));


const uint8_t initdataQT2[]  =
{
  //driving ability
  0x40| 2, 0xEA, 0x00,
  0x40| 2, 0xEB, 0x20,
  0x40| 2, 0xEC, 0x0C,
  0x40| 2, 0xED, 0xC4,
  0x40| 2, 0xE8, 0x40,
  0x40| 2, 0xE9, 0x38,
  0x40| 2, 0xF1, 0x01,
  0x40| 2, 0xF2, 0x10,
  0x40| 2, 0x27, 0xA3,
  //power voltage
  0x40| 2, 0x1B, 0x1B,
  0x40| 2, 0x1A, 0x01,
  0x40| 2, 0x24, 0x2F,
  0x40| 2, 0x25, 0x57,
  //VCOM offset
  0x40| 2, 0x23, 0x8D,
  //power on
  0x40| 2, 0x18, 0x36,
  0x40| 2, 0x19, 0x01, //start osc
  0x40| 2, 0x01, 0x00, //wakeup
  0x40| 2, 0x1F, 0x88,
  0xC0| 5, //5ms
  0x40| 2, 0x1F, 0x80,
  0xC0| 5, //5ms
  0x40| 2, 0x1F, 0x90,
  0xC0| 5, //5ms
  0x40| 2, 0x1F, 0xD0,
  0xC0| 5, //5ms
  //color selection
  0x40| 2, 0x17, 0x05, //0x05=65k, 0x06=262k
  //panel characteristic
  0x40| 2, 0x36, 0x00,
  //display options
  0x40| 2, 0x16, 0xA8,
  0x40| 2, 0x03, 0x00, //x0
  0x40| 2, 0x02, 0x00, //x0
  0x40| 2, 0x05, ((_TFTWIDTH-1)>>0)&0xFF,
  0x40| 2, 0x04, ((_TFTWIDTH-1)>>8)&0xFF,
  0x40| 2, 0x07, 0x00, //y0
  0x40| 2, 0x06, 0x00, //y0
  0x40| 2, 0x09, ((_TFTHEIGHT-1)>>0)&0xFF,
  0x40| 2, 0x08, ((_TFTHEIGHT-1)>>8)&0xFF,
  //display on
  0x40| 2, 0x28, 0x38,
  0xC0|50, //50ms
  0x40| 2, 0x28, 0x3C,
  0xC0| 5, //5ms
  0xFF   , 0xFF
};




void hx8347WritePin(uint8_t pin_num, uint8_t pin_state);
void hx8347InitRegs(void);
void hx8347WriteReg(uint8_t cmd, uint8_t param);




void hx8347Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;


  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = pin_tbl[_PIN_DEF_DC].pin_num;         // DC
  HAL_GPIO_Init(pin_tbl[_PIN_DEF_DC].p_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = pin_tbl[_PIN_DEF_RST].pin_num;         // RST
  HAL_GPIO_Init(pin_tbl[_PIN_DEF_RST].p_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = pin_tbl[_PIN_DEF_CS].pin_num;          // CS
  HAL_GPIO_Init(pin_tbl[_PIN_DEF_CS].p_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = pin_tbl[_PIN_DEF_BL].pin_num;          // BL
  HAL_GPIO_Init(pin_tbl[_PIN_DEF_BL].p_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = pin_tbl[_PIN_DEF_SD].pin_num;          // SD
  HAL_GPIO_Init(pin_tbl[_PIN_DEF_SD].p_port, &GPIO_InitStruct);


  hx8347WritePin(_PIN_DEF_CS,  _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_DC,  _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_RST, _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_BL,  _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_SD,  _DEF_HIGH);


  hx8347WritePin(_PIN_DEF_RST, _DEF_HIGH);
  delay(50);
  hx8347WritePin(_PIN_DEF_RST, _DEF_LOW);
  delay(50);
  hx8347WritePin(_PIN_DEF_RST, _DEF_HIGH);
  delay(50);


  spiBegin(_DEF_SPI1);
  spiSetBitOrder(_DEF_SPI1, SPI_FIRSTBIT_MSB);

  // 200Mhz / div
  spiSetClockDivider(_DEF_SPI1, SPI_BAUDRATEPRESCALER_8); // 25Mhz
  spiSetDataMode(_DEF_SPI1, SPI_MODE0);

  hx8347InitRegs();

}

void hx8347WritePin(uint8_t pin_num, uint8_t pin_state)
{
  if (pin_state == _DEF_HIGH)
  {
    HAL_GPIO_WritePin(pin_tbl[pin_num].p_port, pin_tbl[pin_num].pin_num, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(pin_tbl[pin_num].p_port, pin_tbl[pin_num].pin_num, GPIO_PIN_RESET);
  }
}

void hx8347WriteCommand(uint8_t c)
{
  hx8347WritePin(_PIN_DEF_DC, _DEF_LOW);
  hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

  spiTransfer8(_DEF_SPI1, c);

  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);

}


void hx8347WriteData(uint8_t c)
{
  hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

  spiTransfer8(_DEF_SPI1, c);

  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);
}

void hx8347WriteReg(uint8_t cmd, uint8_t param)
{
  hx8347WritePin(_PIN_DEF_DC, _DEF_LOW);
  hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

  spiTransfer8(_DEF_SPI1, cmd);

  hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);

  spiTransfer8(_DEF_SPI1, param);
  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);
}

void hx8347InitRegs(void)
{
  uint_least8_t c, d, i;
  const uint8_t *ptr;

  //reset
  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);

  //send init commands and data
  ptr = &initdataQT2[0];
  while(1)
  {
    c = *(ptr);
    ptr++;
    if(c == 0xFF) //end of data
    {
      break;
    }
    switch(c&0xC0)
    {
      case 0x40: //command + data
        for(i=c&0x3F; i!=0; i-=2)
        {
          c = *(ptr);
          ptr++;
          d = *(ptr);
          ptr++;

          hx8347WriteReg(c, d);
        }
        break;
      case 0xC0: //delay
        c = c&0x3F;
        delay(c);
        break;
    }
  }
}

uint16_t hx8347GetWidth(void)
{
  return _width;
}

uint16_t hx8347GetHeight(void)
{
  return _height;
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void hx8347SetRotation(uint8_t mode)
{
  uint8_t p = 0x00;


  switch (mode)
  {
    case 0:
      _width  = _TFTWIDTH;
      _height = _TFTHEIGHT;
      colstart = 0;
      rowstart = 0;
      p = 0xA8;
      break;

   case 1:
     _width  = _TFTHEIGHT;
     _height = _TFTWIDTH;
     colstart = 0;
     rowstart = 0;
     p = 0x08;
     break;

   case 2:
     _width  = _TFTWIDTH;
     _height = _TFTHEIGHT;
     colstart = 0;
     rowstart = 0;
     p = 0x68;
     break;

   case 3:
     _width  = _TFTHEIGHT;
     _height = _TFTWIDTH;
     colstart = 0;
     rowstart = 0;
     p = 0xC8;
     break;
  }

  hx8347WriteReg(0x16, p);
  hx8347SetAddrWindow(0, 0,  _width-1, _height-1);

}

void hx8347SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  hx8347WriteReg(0x03, (x0+colstart)>>0);  // XSTART
  hx8347WriteReg(0x02, (x0+colstart)>>8);

  hx8347WriteReg(0x05, (x1+colstart)>>0);  // XEND
  hx8347WriteReg(0x04, (x1+colstart)>>8);

  hx8347WriteReg(0x07, (y0+rowstart)>>0);  // YSTART
  hx8347WriteReg(0x06, (y0+rowstart)>>8);

  hx8347WriteReg(0x09, (y1+rowstart)>>0);  // YEND
  hx8347WriteReg(0x08, (y1+rowstart)>>8);


  hx8347WriteCommand(0x22);         // write to RAM
}

void hx8347drawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w)
{


  //create a local buffer line not to mess up the source
  uint16_t bufferedLine[w];
  for (uint16_t i = 0; i < w; i++) {
    uint16_t color = buffer[i];
    color = (color << 8) | (color >> 8); //change endianness
    bufferedLine[i] = color;
  }

  hx8347SetAddrWindow(x, y, x + w - 1, y + 1);


  hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

  spiDmaTransfer(_DEF_SPI1, bufferedLine, w*2, 100);

  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);
}

void hx8347drawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h)
{


  hx8347SetAddrWindow(x, y, x + w - 1, y + h - 1);


  hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);
  hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

  spiDmaTransfer(_DEF_SPI1, buffer, w*h*2, 100);

  hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);

}

void hx8347sendBuffer(uint16_t *buffer, uint16_t n)
{
  spiDmaTransfer(_DEF_SPI1, buffer, n*2, 0);
}

void hx8347DrawFrame(bool wait)
{
  if (wait == true)
  {
    hx8347SetAddrWindow(0, 0, _width-1, _height-1);

    hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);
    hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);

    spiDmaTransfer(_DEF_SPI1, frame_buf, _width*_height*2, 100);


    hx8347WritePin(_PIN_DEF_CS, _DEF_HIGH);
  }
  else
  {
    hx8347SetAddrWindow(0, 0, _width-1, _height-1);

    hx8347WritePin(_PIN_DEF_DC, _DEF_HIGH);
    hx8347WritePin(_PIN_DEF_CS, _DEF_LOW);


    spiDmaTransfer(_DEF_SPI1, frame_buf, _width*_height*2, 0);
  }
}

void hx8347FillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color)
{
  int32_t x_o = x;
  int32_t y_o = y;

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  uint8_t hi = color >> 8, lo = color;

  color = lo<<8 | hi<<0;


  for(y=0; y<h; y++) {
    for(x=0; x<w; x++) {
      frame_buf[(y_o+y)*_width+(x_o+x)] = color;
    }
  }
}

void hx8347DrawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  frame_buf[y*_width+x] = color>>8 | color<<8;
}

bool hx8347IsBusy(void)
{
  if (spiDmaIsTxDone(_DEF_SPI1) == true)
  {
    return false;
  }
  else
  {
    return true;
  }
}
