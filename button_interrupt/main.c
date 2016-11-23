#include "LPC11XX.h"
#include "timer16.h"
#include "clkconfig.h"
#include "gpio.h"
int mode=0;
int i;
uint32_t interval;
extern volatile uint32_t timer16_0_counter;
#define LED_ON  LPC_GPIO1->DATA &= ~(1<<8)
#define LED_OFF LPC_GPIO1->DATA |= (1<<8)
#define LED_change LPC_GPIO1->DATA ^= (1<<8)
#define KEY_UP (LPC_GPIO2->DATA&(1<<6))==(1<<6)
#define setIE LPC_GPIO2->IE |= (1<<6)
#define setIEV LPC_GPIO2->IEV |= (1<<6)
#define clearIEV LPC_GPIO2->IEV &=~ (1<<6)
#define setIC LPC_GPIO2->IC
#define KEY_interrupt (LPC_GPIO2->RIS&(1<<6))==(1<<6)
#define LED_DIR LPC_GPIO1->DIR |= (1<<8)
#define KEY_DIR LPC_GPIO2->DIR &= ~(1<<6)
void init()
{
		LED_DIR;
		LED_OFF;
		KEY_DIR;
	
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
		init_timer16(0, interval);
		enable_timer16(0);
}
void PIOINT2_IRQHandler(void)
{
		setIEV;
		if(KEY_interrupt)
		{
			for(i=0;i<5000;i++);
			i=0;
			while(i<300000){
				if(KEY_UP)
					break;
				i++;
			}
			if(i==300000){
				while(1){
					i=0;
					while(i<225000){
						i++;
						if(KEY_UP){
							LED_OFF;;
							break;
						}
					}
					LED_OFF;
					i=0;
					while(i<225000){
						i++;
						if(KEY_UP){
							LED_OFF;;
							break;
						}
					}
					LED_ON;
					if(KEY_UP){
						LED_OFF;
						break;
					}
				}
			}
			else if(i<300000){
				LED_change;
			}
			setIC = 0XFFFF;
		}
		clearIEV;
}
int main(void)
{
   init();
   NVIC_EnableIRQ(EINT2_IRQn);
   while(1)
   {
		setIE;
   }
}