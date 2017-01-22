#include "LPC11xx.h"
#include "adc.h"
void ADC_Init(int Channel)
{
   if(Channel>7) return;
   LPC_SYSCON->PDRUNCFG &= ~(0x1<<4);
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
   switch(Channel)
   {
      case 0:
      LPC_IOCON->JTAG_TDI_PIO0_11 &= ~0x07;
      LPC_IOCON->JTAG_TDI_PIO0_11 |= 0x02;
      LPC_IOCON->JTAG_TDI_PIO0_11 &= ~(3<<3);
      LPC_IOCON->JTAG_TDI_PIO0_11 &= ~(1<<7);
      break;
      case 1:  // set channel 1
      LPC_IOCON->JTAG_TMS_PIO1_0 &= ~0x07;
      LPC_IOCON->JTAG_TMS_PIO1_0 |= 0x02;
      LPC_IOCON->JTAG_TMS_PIO1_0 &= ~(3<<3);
      LPC_IOCON->JTAG_TMS_PIO1_0 &= ~(1<<7);
      break;
      case 2:  // set channel 2
      LPC_IOCON->JTAG_TDO_PIO1_1 &= ~0x07;
      LPC_IOCON->JTAG_TDO_PIO1_1 |= 0x02;
      LPC_IOCON->JTAG_TDO_PIO1_1 &= ~(3<<3);
      LPC_IOCON->JTAG_TDO_PIO1_1 &= ~(1<<7);
      break;
      case 3:  // set channel 3
      LPC_IOCON->JTAG_nTRST_PIO1_2 &= ~0x07;
      LPC_IOCON->JTAG_nTRST_PIO1_2 |= 0x02;
      LPC_IOCON->JTAG_nTRST_PIO1_2 &= ~(3<<3);
      LPC_IOCON->JTAG_nTRST_PIO1_2 &= ~(1<<7);
      break;
      case 4:  // set channel 4
      LPC_IOCON->ARM_SWDIO_PIO1_3 &= ~0x07;
      LPC_IOCON->ARM_SWDIO_PIO1_3 |= 0x02;
      LPC_IOCON->ARM_SWDIO_PIO1_3 &= ~(3<<3);
      LPC_IOCON->ARM_SWDIO_PIO1_3 &= ~(1<<7);
      break;
      case 5:  // set channel 5
      LPC_IOCON->PIO1_4 &= ~0x07;
      LPC_IOCON->PIO1_4 |= 0x01;
      LPC_IOCON->PIO1_4 &= ~(3<<3);
      LPC_IOCON->PIO1_4 &= ~(1<<7);
      break;
      case 6:  // set channel 6
      LPC_IOCON->PIO1_10 &= ~0x07;
      LPC_IOCON->PIO1_10 |= 0x01;
      LPC_IOCON->PIO1_10 &= ~(3<<3);
      LPC_IOCON->PIO1_10 &= ~(1<<7);
      break;
      case 7:  // set channel 7
      LPC_IOCON->PIO1_11 &= ~0x07;
      LPC_IOCON->PIO1_11 |= 0x01;
      LPC_IOCON->PIO1_11 &= ~(3<<3);
      LPC_IOCON->PIO1_11 &= ~(1<<7);
      break;
      default:break;
   }
   LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
   LPC_ADC->CR = (1<<Channel)|(24<<8);
}
uint32_t ADC_Read(uint8_t Channel)
{
   float adc_value=0;
   LPC_ADC->CR |= (1<<24);
   while((LPC_ADC->DR[Channel]&0x80000000)==0);
   adc_value = (LPC_ADC->DR[Channel]>>6)&0x3FF;
   return adc_value;
}