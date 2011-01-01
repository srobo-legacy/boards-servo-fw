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

#ifndef __LEDS_H
#define __LEDS_H

/* Debugging and SRIC activity LED next to MSP */
#define LED_DBG_OUT P4OUT
#define LED_DBG (1<<7)

#define LEDSP4 (LED_DBG)


#define leds_init() do { P4OUT &= ~LEDSP4; P4DIR |= LEDSP4; } while(0)

#define __led_set(led_out, led, x) do { if (x) led_out |= led; \
                                        else   led_out &= ~led; } while(0)

#define __led_toggle(led_out, led) do { led_out ^= led; } while(0)

#define dbg_set(x)       __led_set(LED_DBG_OUT, LED_DBG, x)
#define dbg_toggle()     __led_toggle(LED_DBG_OUT, LED_DBG)

#endif /* __LEDS_H */
