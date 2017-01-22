#include "LPC11xx.h"
#include "uart.h"
void UART_init(uint32_t baudrate)
{
   uint32_t DL_value,Clear=Clear;
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
   LPC_IOCON->PIO1_6 &= ~0x07;
   LPC_IOCON->PIO1_6 |= 0x01;
   LPC_IOCON->PIO1_7 &= ~0x07;
   LPC_IOCON->PIO1_7 |= 0x01;
   LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
   LPC_SYSCON->UARTCLKDIV = 0x1;
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
   LPC_UART->LCR = 0x83;
   DL_value = SystemFrequency/16/baudrate;
   LPC_UART->DLM = DL_value / 256;
   LPC_UART->DLL = DL_value % 256;
   LPC_UART->LCR = 0x03;
   LPC_UART->FCR = 0x07;
   Clear = LPC_UART->LSR;
}
uint8_t UART_recive(void)
{
   while(!(LPC_UART->LSR & (1<<0)));
   return(LPC_UART->RBR);
}
void UART_send_byte(uint8_t byte)
{
   LPC_UART->THR = byte;
   while ( !(LPC_UART->LSR & (1<<5)) );
}