#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer.h"
#include "setio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */

static __inline void setdir(void){
	GPIOSetDir(1,8,1);
	GPIOSetDir(2,6,0);
}

static __inline void LED_ON (void){
	GPIOSetValue( 1, 8, 0 );
}

static __inline void LED_OFF (void){
	GPIOSetValue( 1, 8, 1 );
}

static __inline void LED_change (void){
	LPC_GPIO1->DATA^=(1<<8);
}

static __inline int BUTTON_state (void){
	return (LPC_GPIO2->DATA&(1<<6));
}

static __inline void IE_ON (void){
	LPC_GPIO2->IE |= (1<<6);
}

static __inline void IE_OFF (void){
	LPC_GPIO2->IE &=~ (1<<6);
}

static __inline void IEV_ON (void){
	LPC_GPIO2->IEV |= (1<<6);
}

static __inline void IEV_OFF (void){
	LPC_GPIO2->IEV &=~ (1<<6);
}

static __inline int RIS_state (void){
	return (LPC_GPIO2->RIS&(1<<6));
}

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

void init()
{		
		uint32_t interval;

		SystemInit();

  /* TEST_TIMER_NUM is either 0 or 1 for 16-bit timer 0 or 1. */
		interval = SystemAHBFrequency/1000 - 1;
		if ( interval > 0xFFFF )
		{
			interval = 0xFFFF;
		}
		init_timer16(TEST_TIMER_NUM, interval);
		enable_timer16(TEST_TIMER_NUM);
		setdir();
		LED_OFF();
}
void PIOINT2_IRQHandler(void)
{  
	 IE_OFF();
	 IEV_ON();
   if(RIS_state()==(1<<6))
   {	
		 delayMs(1,50);
		 if(BUTTON_state()!=(1<<6)){
      timer16_0_counter = 0;
			while(1){
				if((timer16_0_counter > 500&&BUTTON_state()!=(1<<6))){
					LED_OFF();
					timer16_0_counter = 0;
					while(1){
						if ( (timer16_0_counter > 0) && (timer16_0_counter <= 250)&&BUTTON_state()!=(1<<6)){
							LED_OFF();
						}else if ( (timer16_0_counter > 250) && (timer16_0_counter <= 500)&&BUTTON_state()!=(1<<6)){
							LED_ON();
						}else if((timer16_0_counter > 500)&&BUTTON_state()!=(1<<6)){
							timer16_0_counter = 0;
						}
						if(BUTTON_state()==(1<<6)){
							LED_OFF();
							break;
						}
					}
					break;
				}else if(timer16_0_counter < 500&&BUTTON_state()==(1<<6)){
					LED_change();
					break;
				}
			}
		}
      LPC_GPIO2->IC = 0XFFFF;
   }
	 IEV_OFF();
	 IE_ON();
}
int main()
{  
	 init();
   NVIC_EnableIRQ(EINT2_IRQn);
	 NVIC_SetPriority(TIMER_16_0_IRQn,0);
	 NVIC_SetPriority(EINT2_IRQn,3);
   while(1)
   {
		IE_ON();
   }
}