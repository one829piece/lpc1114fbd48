#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer16.h"
#include "clkconfig.h"
#include "gpio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */
#define LED_ON GPIOSetValue( 2, 7, 0 )
#define LED_OFF GPIOSetValue( 2, 7, 1 )
#define LED_DIR LPC_GPIO2->DIR|=(1<<7)
#define BUTTON_DIR	LPC_GPIO2->DIR&=~(1<<6)
#define BUTTON_press (LPC_GPIO2->DATA&(1<<6))!=(1<<6)
#define BUTTON_on (LPC_GPIO2->DATA&(1<<6))==(1<<6)
#define LED_change LPC_GPIO2->DATA^=(1<<7);

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

int i=0;

int main(void){
	button_interrupt(250,500);
}

int button_interrupt(int blinkytime,int presstime){
	uint32_t interval;

	SystemInit();
	
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
	
	LED_DIR;
	BUTTON_DIR;
	
	while(1){
		if(BUTTON_press){
			delayMs(1,20);
			timer16_0_counter = 0;
			while(1){
				delayMs(1,20);
				if((timer16_0_counter > presstime&&BUTTON_press)){
					delayMs(1,20);
					LED_OFF;
					timer16_0_counter = 0;
					while(1){
						if ( (timer16_0_counter > 0) && (timer16_0_counter <= blinkytime)&&BUTTON_press){
							LED_OFF;
						}else if ( (timer16_0_counter > blinkytime) && (timer16_0_counter <= blinkytime*2)&&BUTTON_press){
							LED_ON;
						}else if((timer16_0_counter > blinkytime*2)&&BUTTON_press){
							timer16_0_counter = 0;
						}
						delayMs(1,20);
						if(BUTTON_on){
							LED_OFF;
							break;
						}
					}
					break;
				}else if(timer16_0_counter < presstime&&BUTTON_on){
					delayMs(1,20);
					LED_change;
					break;
				}
			}
		}
	}
}