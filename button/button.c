#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer.h"
#include "setio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

int i=0;

static __inline void setdir(void){
	GPIOSetDir(2,7,1);
	GPIOSetDir(2,6,0);
}

static __inline void LED_ON (void){
	GPIOSetValue( 2, 7, 0 );
}

static __inline void LED_OFF (void){
	GPIOSetValue( 2, 7, 1 );
}

static __inline void LED_change (void){
	LPC_GPIO2->DATA^=(1<<7);
}

static __inline int BUTTON_state (void){
	return (LPC_GPIO2->DATA&(1<<6));
}

int main(void){
	button_interrupt(250,500);
}

int button_interrupt(int blinkytime,int presstime){
	uint32_t interval;

	SystemInit();
	
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  interval = SystemAHBFrequency/1000 - 1;
  if ( interval > 0xFFFF )
  {
	interval = 0xFFFF;
  }
	
  init_timer16(TEST_TIMER_NUM, interval);
  enable_timer16(TEST_TIMER_NUM);
	
	setdir();
	
	while(1){
		if(BUTTON_state()!=(1<<6)){
			delayMs(1,20);
			timer16_0_counter = 0;
			while(1){
				delayMs(1,20);
				if((timer16_0_counter > presstime&&BUTTON_state()!=(1<<6))){
					delayMs(1,20);
					LED_OFF();
					timer16_0_counter = 0;
					while(1){
						if ( (timer16_0_counter > 0) && (timer16_0_counter <= blinkytime)&&BUTTON_state()!=(1<<6)){
							LED_OFF();
						}else if ( (timer16_0_counter > blinkytime) && (timer16_0_counter <= blinkytime*2)&&BUTTON_state()!=(1<<6)){
							LED_ON();
						}else if((timer16_0_counter > blinkytime*2)&&BUTTON_state()!=(1<<6)){
							timer16_0_counter = 0;
						}
						delayMs(1,20);
						if(BUTTON_state()==(1<<6)){
							LED_OFF();
							break;
						}
					}
					break;
				}else if(timer16_0_counter < presstime&&BUTTON_state()==(1<<6)){
					delayMs(1,20);
					LED_change();
					break;
				}
			}
		}
	}
}