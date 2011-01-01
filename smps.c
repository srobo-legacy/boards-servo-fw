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

#include "smps.h"
#include <io.h>

/* This signal is active-low */
#define SMPS_PIN (1<<0)

void smps_init(void) {
	P2SEL &= ~SMPS_PIN;
	P2OUT |= SMPS_PIN;
	P2DIR |= SMPS_PIN;
}

void smps_en(void) {
	P2OUT &= ~SMPS_PIN;
}

void smps_dis(void) {
	P2OUT |= SMPS_PIN;
}
