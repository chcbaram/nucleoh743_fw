/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "ap.h"


void testCmdif(void);


void apInit(void)
{
  cmdifOpen(_DEF_UART1, 115200);

  cmdifAdd("test", testCmdif);
}

void apMain(void)
{
  while(1)
  {
    cmdifLoop();
  }
}


void testCmdif(void)
{
  uint8_t number;
  bool ret = true;


  if (cmdifHasString("led", 0) == true)
  {
    number = (uint8_t)cmdifGetParam(1);

    if (number > 0)
    {
      number -= 1;
    }

    while(cmdifRxAvailable() == 0)
    {
      ledToggle(number);
      delay(100);
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cmdifPrintf( "test led <1~3>\n");
  }
}
