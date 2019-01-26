/*
 * ap.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: Baram
 */




#include "ap.h"



void apInit(void)
{
}

void apMain(void)
{
  while(1)
  {
    if (vcpAvailable() > 0)
    {
      vcpPrintf("vcp rx : 0x%X\n", vcpRead());
    }
  }
}
