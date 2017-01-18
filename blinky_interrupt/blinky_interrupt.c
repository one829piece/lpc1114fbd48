#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer.h"
#include "setio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

void blink_interrupt(uint32_t portNum,uint32_t bitPosi,uint32_t on_time,uint32_t off_time){
	uint32_t interval;

	SystemInit();

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

  /* Set port 0_7 to output */
   GPIOSetDir( portNum, bitPosi, 1 );

  while (1)                                /* Loop forever */
  {
#if TEST_TIMER_NUM
	/* I/O configuration and LED setting pending. */
	if ( (timer16_1_counter > 0) && (timer16_1_counter <= on_time) )
	{
	  GPIOSetValue( portNum, bitPosi, 0 );
	}
	if ( (timer16_1_counter > on_time) && (timer16_1_counter <= on_time+off_time) )
	{
	  GPIOSetValue( portNum, bitPosi, 1 );
	}
	else if ( timer16_1_counter > on_time+off_time )
	{
	  timer16_1_counter = 0;
	}
#else
	/* I/O configuration and LED setting pending. */
	if ( (timer16_0_counter > 0) && (timer16_0_counter <= on_time) )
	{
	  GPIOSetValue( portNum, bitPosi, 0 );
	}
	if ( (timer16_0_counter > on_time) && (timer16_0_counter <= on_time+off_time) )
	{
	  GPIOSetValue( portNum, bitPosi, 1 );
	}
	else if ( timer16_0_counter > on_time+off_time )
	{
	  timer16_0_counter = 0;
	}
#endif
  }
}