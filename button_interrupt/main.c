#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer16.h"
#include "clkconfig.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */
#define LED_ON LPC_GPIO1->DATA &= ~(1<<8)
#define LED_OFF LPC_GPIO1->DATA |= (1<<8)
#define LED_DIR LPC_GPIO1->DIR|=(1<<8)
#define BUTTON_DIR	LPC_GPIO2->DIR&=~(1<<6)
#define BUTTON_press (LPC_GPIO2->DATA&(1<<6))!=(1<<6)
#define BUTTON_on (LPC_GPIO2->DATA&(1<<6))==(1<<6)
#define LED_change LPC_GPIO1->DATA^=(1<<8);

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

void init()
{		
		uint32_t interval;

		SystemInit();

  /* Config CLKOUT, mostly used for debugging. */
		CLKOUT_Setup( CLKOUTCLK_SRC_MAIN_CLK );
		LPC_IOCON->PIO0_1 &= ~0x07;	
		LPC_IOCON->PIO0_1 |= 0x01;		/* CLK OUT */

  /* Enable AHB clock to the GPIO domain. */
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  /* TEST_TIMER_NUM is either 0 or 1 for 16-bit timer 0 or 1. */
		interval = SystemAHBFrequency/1000 - 1;
		if ( interval > 0xFFFF )
		{
			interval = 0xFFFF;
		}
		init_timer16(TEST_TIMER_NUM, interval);
		enable_timer16(TEST_TIMER_NUM);
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
		LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
		LED_DIR;
		LED_OFF;
		BUTTON_DIR;
}
void PIOINT2_IRQHandler(void)
{  LPC_GPIO2->IEV |= (1<<6);
   if((LPC_GPIO2->RIS&(1<<6))==(1<<6))
   {	
		 delayMs(1,50);
		 if((LPC_GPIO2->DATA&(1<<6))!=(1<<6)){
      timer16_0_counter = 0;
			while(1){
				if((timer16_0_counter > 500&&BUTTON_press)){
					LED_OFF;
					timer16_0_counter = 0;
					while(1){
						if ( (timer16_0_counter > 0) && (timer16_0_counter <= 250)&&BUTTON_press){
							LED_OFF;
						}else if ( (timer16_0_counter > 250) && (timer16_0_counter <= 500)&&BUTTON_press){
							LED_ON;
						}else if((timer16_0_counter > 500)&&BUTTON_press){
							timer16_0_counter = 0;
						}
						if(BUTTON_on){
							LED_OFF;
							break;
						}
					}
					break;
				}else if(timer16_0_counter < 500&&BUTTON_on){
					LED_change;
					break;
				}
			}
		}
      LPC_GPIO2->IC = 0XFFFF;
   }
	 LPC_GPIO2->IEV &= ~(1<<6);
}
int main()
{  init();
   NVIC_EnableIRQ(EINT2_IRQn);
	 NVIC_SetPriority(TIMER_16_0_IRQn,0);
	 NVIC_SetPriority(EINT2_IRQn,3);
   while(1)
   {
		LPC_GPIO2->IE |= (1<<6);
   }
}