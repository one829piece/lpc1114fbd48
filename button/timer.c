#include "LPC11xx.h"
#include "timer.h"

volatile uint32_t timer16_0_counter = 0;
volatile uint32_t timer16_1_counter = 0;
volatile uint32_t timer16_0_capture = 0;
volatile uint32_t timer16_1_capture = 0;
volatile uint32_t timer16_0_period = 0;
volatile uint32_t timer16_1_period = 0;

void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if (timer_num == 0)
  {
    /*
    * setup timer #0 for delay
    */
    LPC_TMR16B0->TCR = 0x02;		/* reset timer */
    LPC_TMR16B0->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR16B0->MR0 = delayInMs * (SystemAHBFrequency / 1000);
    LPC_TMR16B0->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR16B0->MCR = 0x04;		/* stop timer on match */
    LPC_TMR16B0->TCR = 0x01;		/* start timer */
    /* wait until delay time has elapsed */
    while (LPC_TMR16B0->TCR & 0x01);
  }
  else if (timer_num == 1)
  {
    /*
    * setup timer #1 for delay
    */
    LPC_TMR16B1->TCR = 0x02;		/* reset timer */
    LPC_TMR16B1->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR16B1->MR0 = delayInMs * (SystemAHBFrequency / 1000);
    LPC_TMR16B1->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR16B1->MCR = 0x04;		/* stop timer on match */
    LPC_TMR16B1->TCR = 0x01;		/* start timer */
    /* wait until delay time has elapsed */
    while (LPC_TMR16B1->TCR & 0x01);
  }
  return;
}

void TIMER16_0_IRQHandler(void)
{  
  if ( LPC_TMR16B0->IR & 0x1 )
  {
	LPC_TMR16B0->IR = 1;			/* clear interrupt flag */
	timer16_0_counter++;
  }
  if ( LPC_TMR16B0->IR & (0x1<<4) )
  {
	LPC_TMR16B0->IR = 0x1<<4;		/* clear interrupt flag */
	timer16_0_capture++;
  }
  return;
}

void TIMER16_1_IRQHandler(void)
{
  if ( LPC_TMR16B1->IR & 0x1 )
  {  
	LPC_TMR16B1->IR = 1;			/* clear interrupt flag */
	timer16_1_counter++;
  }
  if ( LPC_TMR16B1->IR & (0x1<<4) )
  {
	LPC_TMR16B1->IR = 0x1<<4;		/* clear interrupt flag */
	timer16_1_capture++;
  }
  return;
}

void enable_timer16(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_TMR16B0->TCR = 1;
  }
  else
  {
    LPC_TMR16B1->TCR = 1;
  }
  return;
}

void init_timer16(uint8_t timer_num, uint32_t TimerInterval) 
{
	if ( timer_num == 0 )
	{
		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
		LPC_IOCON->PIO0_2           &= ~0x07;	/*  Timer0_16 I/O config */
		LPC_IOCON->PIO0_2           |= 0x02;		/* Timer0_16 CAP0 */
		LPC_IOCON->PIO0_8           &= ~0x07;	
		LPC_IOCON->PIO0_8           |= 0x02;		/* Timer0_16 MAT0 */
		LPC_IOCON->PIO0_9           &= ~0x07;
		LPC_IOCON->PIO0_9           |= 0x02;		/* Timer0_16 MAT1 */

#ifdef __JTAG_DISABLED
		LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;
		LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x03;		/* Timer0_16 MAT2 */
#endif	
		
		timer16_0_counter = 0;
		timer16_0_capture = 0;
		LPC_TMR16B0->MR0 = TimerInterval;
#if TIMER_MATCH
		LPC_TMR16B0->EMR &= ~(0xFF<<4);
		LPC_TMR16B0->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8));
#else
		/* Capture 0 on rising edge, interrupt enable. */
		LPC_TMR16B0->CCR = (0x1<<0)|(0x1<<2);
#endif
		LPC_TMR16B0->MCR = 3;				/* Interrupt and Reset on MR0 and MR1 */
		
		/* Enable the TIMER0 Interrupt */
		NVIC_EnableIRQ(TIMER_16_0_IRQn);
		}
	else if ( timer_num == 1 )
	{
		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
		LPC_IOCON->PIO1_8           &= ~0x07;	/*  Timer1_16 I/O config */
		LPC_IOCON->PIO1_8           |= 0x01;		/* Timer1_16 CAP0 */
		LPC_IOCON->PIO1_9           &= ~0x07;	
		LPC_IOCON->PIO1_9           |= 0x01;		/* Timer1_16 MAT0 */
		LPC_IOCON->PIO1_10          &= ~0x07;
		LPC_IOCON->PIO1_10          |= 0x02;		/* Timer1_16 MAT1 */	
		
		timer16_1_counter = 0;
		timer16_1_capture = 0;
		LPC_TMR16B1->MR0 = TimerInterval;
#if TIMER_MATCH
		LPC_TMR16B1->EMR &= ~(0xFF<<4);
		LPC_TMR16B1->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8));
#else
		/* Capture 0 on rising edge, interrupt enable. */
		LPC_TMR16B1->CCR = (0x1<<0)|(0x1<<2);
#endif
		LPC_TMR16B1->MCR = 3;				/* Interrupt and Reset on MR0 and MR1 */
		
		/* Enable the TIMER1 Interrupt */
		NVIC_EnableIRQ(TIMER_16_1_IRQn);
	}
	return;
}