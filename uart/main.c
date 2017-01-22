#include "LPC11xx.h"
#include "uart.h"
#include "PWM.h"
#include "adc.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */

extern volatile uint32_t timer16_0_counter;

char rec_buf;
int i,z,j,k;
char adc[6];
float x;
int status=0;

void UART_IRQHandler(void)
{
   uint32_t IRQ_ID;
   uint8_t rec_buf;
   IRQ_ID = LPC_UART->IIR;
   IRQ_ID =((IRQ_ID>>1)&0x7);
   if(IRQ_ID == 0x02 )
   {
      rec_buf = (LPC_UART->RBR);
		  
			if(rec_buf=='$'){
				status=1;
			}else if(rec_buf=='#'){
				status=0;
			}
   }
}

void UART_printf(char* string) {
	while (*string) {
		UART_send_byte(*string++);
	}
}	//end of UART_printf()

int main()
{
	uint32_t interval;
	
  ADC_Init(7);
  SystemInit();
	
  UART_init(9600);
  LPC_UART->IER = 0x01;
  NVIC_EnableIRQ(UART_IRQn);
	
	/* TEST_TIMER_NUM is either 0 or 1 for 16-bit timer 0 or 1. */
 interval = SystemAHBFrequency/1000 - 1;
 if ( interval > 0xFFFF )
 {
   interval = 0xFFFF;
 }
 init_timer16(TEST_TIMER_NUM, interval);
 enable_timer16(TEST_TIMER_NUM);

	cycle = 1000;
  duty = 99;
 
  timer16_0_counter = 0;
	
  while(1)
  {
    x=ADC_Read(7);
		
		if(status==1){
			TIM32B1_PWM(cycle,(x/1023)*99);
		}
		
		if(timer16_0_counter > 2000){
			
			 /*Place the ADC in the array*/
			 for ( z = 0; x > 100; z++)
			 {
				 x = x - 100;
			 }

			 for ( j = 0; x > 10; j++)
			 {
				 x = x - 10;
			 }

			 for ( k = 0; x > 1; k++)
			 {
				 x = x - 1;
			 }
		
			 adc[0]=i+'0';
			 adc[1]=z+'0';
			 adc[2]=j+'0';
			 adc[3]=k+'0';
			 adc[4]='\r';
			 adc[5]='\n';
		
			 UART_printf(adc);
			 timer16_0_counter = 0;
		}
  }
}