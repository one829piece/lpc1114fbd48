#include "LPC11xx.h"     

#ifndef __blinky_interrupt_H__
#define __blinky_interrupt_H__

void blink_interrupt(uint32_t portNum,uint32_t bitPosi,uint32_t on_time,uint32_t off_time);

#endif