#include "LPC11XX.h"
#include "blinky.h"
#include "setio.h"

volatile uint32_t i = 0;

void blinky(uint32_t portNum, uint32_t bitPosi, uint32_t time){

	SystemInit();
	
	/* debug PLL after configuration. */
	LPC_SYSCON->CLKOUTCLKSEL = 0x03;	/* Select Main clock */
	LPC_SYSCON->CLKOUTUEN = 0x01;		/* Update clock */
	LPC_SYSCON->CLKOUTUEN = 0x00;		/* Toggle update register once */
	LPC_SYSCON->CLKOUTUEN = 0x01;
	while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) );		/* Wait until updated */
  	LPC_SYSCON->CLKOUTDIV = 1;			/* Divided by 1 */
	
 	LPC_IOCON->PIO0_1 &= ~0x07;	
 	LPC_IOCON->PIO0_1 |= 0x01;		/* CLK OUT */

	GPIOSetDir( portNum, bitPosi, 1 );
	while(1){
		GPIOSetValue( portNum, bitPosi, 1 );
		for(i=0;i<=time;i++);
		GPIOSetValue( portNum, bitPosi, 0 );
		for(i=0;i<=time;i++);
	}
}