/****************************************************************************
 *   $Id:: blinky.c 9360 2012-04-19 22:33:59Z nxp41306                      $
 *   Project: NXP LPC11xx Blinky example
 *
 *   Description:
 *     This file contains LED blink code example which include timer, 
 *     GPIO initialization, and clock monitoring.
 *
 ****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors? 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.

****************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "blinky_interrupt.h"

/* Main Program */

int main (void) {
  blink_interrupt(2,7,1000,250);
}