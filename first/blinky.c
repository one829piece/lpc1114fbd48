#include "LPC11XX.h"
#include "blinky.h"
#include "gpio.h"

volatile uint32_t i = 0;

void blinky(uint32_t portNum, uint32_t bitPosi, uint32_t time){
	GPIOSetDir( portNum, bitPosi, 1 );
	while(1){
		GPIOSetValue( portNum, bitPosi, 1 );
		for(i=0;i<=time;i++);
		GPIOSetValue( portNum, bitPosi, 0 );
		for(i=0;i<=time;i++);
	}
}