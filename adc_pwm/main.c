#include "lpc11xx.h"
#include "PWM.h"
#include "adc.h"
#include "timer.h"

volatile uint32_t ADCValue;
float x;

int main()
{
	ADC_Init(7);
	SystemInit();

  cycle = 1000;
  duty = 99;
	while(1){
		x=ADC_Read(7);
		delayMs(1,10);
		TIM32B1_PWM(cycle,(x/1023)*99);
	}
}