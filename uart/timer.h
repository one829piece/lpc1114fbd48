#include "LPC11xx.h"

#ifndef __TIMER_H 
#define __TIMER_H

void delayMs(uint8_t timer_num, uint32_t delayInMs);
void TIMER16_0_IRQHandler(void);
void TIMER16_1_IRQHandler(void);
void enable_timer16(uint8_t timer_num);
void init_timer16(uint8_t timer_num, uint32_t timerInterval);

#endif