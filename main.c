/*   Copyright (C) 2010 Richard Barlow, Robert Spanton

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
#include <stddef.h>
#include "drivers/sched.h"
#include "drivers/usci.h"
#include "drivers/pinint.h"
#include "libsric/sric.h"
#include "libsric/sric-client.h"
#include "libsric/token-10f.h"

#include "leds.h"
#include "smps.h"
#include "servo.h"

const usci_t usci_config[1] = {
	{
		.tx_gen_byte = sric_tx_cb,
		.rx_byte = sric_rx_cb,

		/* 115200 baud
		   from http://mspgcc.sourceforge.net/baudrate.html */
		.br0 = 0x8A,
		.br1 = 0x00,
		.mctl = 0xee,

		.sel_rx = &P3SEL,
		.sel_tx = &P3SEL,
		.sel_rx_num = 5,
		.sel_tx_num = 4,
	},
};

const sric_conf_t sric_conf = {
	.usart_tx_start = usci_tx_start,
	.usart_rx_gate = usci_rx_gate,
	.usart_n = 0,

	.txen_dir = &P3DIR,
	.txen_port = &P3OUT,
	.txen_mask = (1<<6),

	.rx_cmd = sric_client_rx,
	.rx_resp = NULL,
	.error = NULL,
	.token_drv = &token_10f_drv,
};

const token_10f_conf_t token_10f_conf = {
	.haz_token = sric_haz_token,

	.gt_port = &P3OUT,
	.gt_dir = &P3DIR,
	.gt_mask = (1<<7),

	.ht_port = &P2IN,
	.ht_dir = &P2DIR,
	.ht_mask = (1<<3),
};

const sric_client_conf_t sric_client_conf = {
	.devclass = SRIC_CLASS_SERVO,
};

void init(void) {
	leds_init();

	/* Set DCO to 16MHz */
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;

	smps_init();
	servo_init();

	pinint_init();
	sched_init();
	usci_init();

	sric_init();
	sric_client_init();
	token_10f_init();

	eint();
}

int main(void) {
	/* Disable watchdog */
	WDTCTL = WDTHOLD | WDTPW;

	init();

	while(1) {
		sric_poll();
	}
}
