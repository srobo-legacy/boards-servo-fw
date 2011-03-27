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

#include "cmds.h"
#include "servo.h"
#include "flash430/sric-flash.h"

/* Set servo position
 * Arguments:
 *   - uint8_t: servo; 0 to 7
 *   - uint16_t: position; 0 to 400 */
static uint8_t sric_set_servo(const sric_if_t *iface);
/* Get current servo position
 * Arguments:
 *   - uint8_t: servo; 0 to 7
 * Returns:
 *   - uint16_t: position; 0 to 400 */
static uint8_t sric_get_servo(const sric_if_t *iface);

const sric_cmd_t sric_commands[] = {
	{sric_set_servo},
	{sric_get_servo},
	{sric_flashr_fw_ver},
	{sric_flashw_fw_chunk},
	{sric_flashr_fw_next},
	{sric_flashr_crc},
	{sric_flashw_confirm},
};

const uint8_t sric_cmd_num = sizeof(sric_commands) / sizeof(const sric_cmd_t);

static uint8_t sric_set_servo(const sric_if_t *iface) {
	uint8_t servo = iface->rxbuf[SRIC_DATA + 1];
	uint16_t pos = iface->rxbuf[SRIC_DATA + 2];
	pos |= iface->rxbuf[SRIC_DATA + 3] << 8;
	servo_set(servo, pos);
	return 0;
}

static uint8_t sric_get_servo(const sric_if_t *iface) {
	uint8_t servo = iface->rxbuf[SRIC_DATA + 1];
	uint16_t p = servo_get(servo);
	iface->txbuf[SRIC_DATA] = p & 0xff;
	iface->txbuf[SRIC_DATA] = (p>>8) & 0xff;
	return 1;
}
