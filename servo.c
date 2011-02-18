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

/* Pulse width in us */
#define MIN_PULSE 600
#define MAX_PULSE 2400

/* Tick width in us */
#define TICK 0.125

/* Min/Max number of ticks per pulse */
#define MIN_TICK (MIN_PULSE/TICK)
#define MAX_TICK (MAX_PULSE/TICK)

#define TICK_PER_STEP ((uint8_t)((MAX_TICK-MIN_TICK)/SERVO_STEPS))

#define step_to_tick(step) (MIN_TICK+(TICK_PER_STEP*step))
#define tick_to_step(tick) ((tick-MIN_TICK)/TICK_PER_STEP)

/* Current servo being controlled */
static uint8_t curr_servo;
/* Current positions of the servos, stored as ticks */
static uint16_t position[8];

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
		servo_set(i, SERVO_MID);
	}
	curr_servo = 0;

	/* Setup pins */
	P4SEL |= 1;              /* P4.0 is the timer/enable output */
	P4DIR |= 1;              /* P4.0 is an output */
	P4OUT &= ~(7<<2);
	P4DIR |= (7<<2);         /* P4.{2,3,4} are outputs */

	/* Setup Timer B */
	TBCTL |= TBCLR;          /* Clear timer settings */
	TBCTL |= TBSSEL_SMCLK    /* Source clock from SMCLK (16MHz) */
	       | ID_DIV2;        /* Divide SMCLK by 2 */

	TBCCTL0 |= OUTMOD_OUT;   /* Set output mode to 'OUT bit value' */
	TBCCTL0 |= OUT;          /* Set CCR0 ouput high, start of a pulse */

	TBCCTL0 |= OUTMOD_RESET  /* Set oupput mode to 'Reset' */
	         | CCIE;         /* Enable interrupt on CCR0 */

	TBCCR0 = position[curr_servo];

	/* and off we go */
	TBR = 0;
	TBCTL |= MC_CONT;
}

void servo_set(uint8_t servo, uint16_t pos) {
	if (servo > 7)
		return;

	if (pos > SERVO_STEPS)
		pos = SERVO_STEPS;

	position[servo] = step_to_tick(pos);
}

uint16_t servo_get(uint8_t servo) {
	if (servo > 7)
		return SERVO_ERR;

	return tick_to_step(position[servo]);
}
