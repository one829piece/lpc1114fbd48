#include "LPC11xx.h"
#include "setio.h"

volatile uint32_t GPIOShadowPort0;
volatile uint32_t GPIOShadowPort1;
volatile uint32_t GPIOShadowPort2;
volatile uint32_t GPIOShadowPort3;

void GPIOInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

#ifdef __JTAG_DISABLED  
  LPC_IOCON->JTAG_TDO_PIO1_1  &= ~0x07;
  LPC_IOCON->JTAG_TDO_PIO1_1  |= 0x01;
#endif

  return;
}

void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  /* if DIR is OUT(1), but GPIOx_DIR is not set, set DIR
  to OUT(1); if DIR is IN(0), but GPIOx_DIR is set, clr
  DIR to IN(0). All the other cases are ignored. 
  On port3(bit 0 through 3 only), no error protection if 
  bit value is out of range. */
  switch ( portNum )
  {
	case PORT0:
	  if ( !(LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO0->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO0->DIR &= ~(0x1<<bitPosi);	  
	break;
 	case PORT1:
	  if ( !(LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO1->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO1->DIR &= ~(0x1<<bitPosi);	  
	break;
	case PORT2:
	  if ( !(LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO2->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO2->DIR &= ~(0x1<<bitPosi);	  
	break;
	case PORT3:
	  if ( !(LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO3->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO3->DIR &= ~(0x1<<bitPosi);	  
	break;
	default:
	  break;
  }
  return;
}

void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  /* if bitVal is 1, the bitPosi bit is set in the GPIOShadowPortx. Then
   * GPIOShadowPortx is written to the I/O port register. */
  switch ( portNum )
  {
	case PORT0:
	  if(bitVal)
		GPIOShadowPort0 |= (1<<bitPosi);					 
	  else
		GPIOShadowPort0 &= ~(1<<bitPosi);

	  /* Use of shadow prevents bit operation error if the read value
	  * (external hardware state) of a pin differs from the I/O latch
	  * value. A potential side effect is that other GPIO code in this
	  * project that is not aware of the shadow will have its GPIO
	  * state overwritten.
	  */
	 LPC_GPIO0->DATA = GPIOShadowPort0;
	break;
 	case PORT1:
	 if(bitVal)
		 GPIOShadowPort1 |= (1<<bitPosi);
	 else
		 GPIOShadowPort1 &= ~(1<<bitPosi);

	 LPC_GPIO1->DATA = GPIOShadowPort1;
	break;
	case PORT2:
	 if(bitVal)
		 GPIOShadowPort2 |= (1<<bitPosi);
	 else
		 GPIOShadowPort2 &= ~(1<<bitPosi);

	 LPC_GPIO2->DATA = GPIOShadowPort2;
	break;
	case PORT3:
	 if(bitVal)
		 GPIOShadowPort3 |= (1<<bitPosi);
	 else
		 GPIOShadowPort3 &= ~(1<<bitPosi);

	 LPC_GPIO3->DATA = GPIOShadowPort3;
	break;
	default:
	  break;
  }
  return;
}