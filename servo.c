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
#include "servo.h"

/* Current servo being controlled */
static uint8_t curr_servo;
/* Current positions of the servos */
static uint8_t position[8];

void servo_init(void) {
	int i;
	for (i = 0; i < 8; i++) {
		position[i] = SERVO_MID;
	}
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
