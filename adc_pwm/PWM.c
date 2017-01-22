#include "lpc11xx.h"
#include "PWM.h"


uint16_t cycle;
uint8_t duty;
void T16B0M0_PWM_Init(void)
{
   if((duty>=100)&&(duty<=0))return;
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
   LPC_IOCON->PIO0_9 &= ~0x07;
   LPC_IOCON->PIO0_9 |= 0x02;
   LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
   LPC_TMR16B0->TCR = 0x02;
   LPC_TMR16B0->PR  = SystemFrequency/1000000-1;
   LPC_TMR16B0->PWMC= 0x01;
   LPC_TMR16B0->MCR = 0x02<<9;
   LPC_TMR16B0->MR3 = cycle;
   LPC_TMR16B0->MR0 = cycle/100*(100-duty);
   LPC_TMR16B0->TCR = 0x01;
}
void T16B0M0_PWM_SET(void)
{
   if((duty>=100)&&(duty<=0))return;
   LPC_TMR16B0->MR0 = cycle/100*(100-duty);
}

void TIM32B1_PWM(uint16_t cycle,uint8_t duty)
{
if((duty>=100)&&(duty<=0))return;
	
		LPC_SYSCON->SYSAHBCLKCTRL |=(1<<10);
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
    LPC_IOCON->JTAG_TDO_PIO1_1 &= ~0x07; //clear
    LPC_IOCON->JTAG_TDO_PIO1_1 |= 0x03; // P1.6 Timer0_32 MAT0
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); 
    LPC_TMR32B1->TCR = 0x00; //TCR(Timer Control Register) the counters are disabled
    LPC_TMR32B1->TCR = 0x02; // The counters remain reset until TCR[1] is returned to zero.
		LPC_TMR32B1->PWMC= 0x01; // PWM mode is enabled for CT32B0_MAT0
    LPC_TMR32B1->MCR = 0x02<<9; //Reset on MR3: the TC will be reset if MR3 matches it.
    LPC_TMR32B1->MR3 = cycle; // set cycle
    LPC_TMR32B1->MR0 = cycle*(100-duty)/100;  //duty cycle
    LPC_TMR32B1->TCR = 0x01; //start
}