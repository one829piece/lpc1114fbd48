#include "LPC11xx.h"

#ifndef __setio_H__
#define __setio_H__

#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3

void GPIOInit( void );
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );

#endif