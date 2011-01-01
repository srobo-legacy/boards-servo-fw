/*   Copyright (C) 2010 Richard Barlow

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <io.h>
#include <signal.h>
#include "servo.h"

/* Current servo being controlled */
static uint8_t curr_servo;
/* Current positions of the servos */
static uint8_t position[8];

interrupt (TIMERB0_VECTOR) servo_timer_isr(void) {
	/* Stop timer while we fiddle with it */
	TBCTL &= ~MC_3;

	/* Move onto the next servo */
	curr_servo++;
	if (curr_servo == 8)
		curr_servo = 0;

	/* Set the decoder output to the correct servo */
	P4OUT &= ~(7<<2);
	P4OUT |= curr_servo << 2;

	/* Enable the decoder, start of the pulse */
	TBCCTL0 &= ~OUTMOD_7;
	TBCCTL0 |= OUTMOD_OUT;
	TBCCTL0 |= OUT;

	/* Get everything setup to disable the decoder in a bit*/
	TBCCTL0 |= OUTMOD_RESET;
	TBCCR0 = position[curr_servo];

	/* Start timer running again */
	TBR = 0;
	TBCTL |= MC_CONT;
}

void servo_init(void) {
	int i;
	for (i = 0; i < 8; i++) {
		position[i] = SERVO_MID;
	}
	curr_servo = 0;

	/* Setup pins */
	P4SEL |= 1;              /* P4.0 is the timer/enable output */
	P4OUT &= ~(7<<2);
	P4DIR |= (7<<2);         /* P4.{2,3,4} are outputs */

	/* Setup Timer B */
	TBCTL |= TBCLR;          /* Clear timer settings */
	TBCTL |= TBSSEL_SMCLK    /* Source clock from SMCLK (16MHz) */
	       | ID_DIV8;        /* Divide SMCLK by 8 */

	TBCCTL0 |= OUTMOD_OUT;   /* Set output mode to 'OUT bit value' */
	TBCCTL0 |= OUT;          /* Set CCR0 ouput high, start of a pulse */

	TBCCTL0 |= OUTMOD_RESET  /* Set oupput mode to 'Reset' */
	         | CCIE;         /* Enable interrupt on CCR0 */

	/* TODO change 'position' to hold the number of ticks rather than
	 * the 0-100 angle setting */
	TBCCR0 = position[curr_servo];

	/* and off we go */
	TBR = 0;
	TBCTL |= MC_CONT;
}

void servo_set(uint8_t servo, uint8_t pos) {
	if (servo > 7)
		return;

	if (pos > SERVO_MAX || pos < SERVO_MIN)
		return;

	position[servo] = pos;
}

uint8_t servo_get(uint8_t servo) {
	if (servo > 7)
		return SERVO_ERR;

	return position[servo];
}
