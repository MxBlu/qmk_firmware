/* Copyright 2020 Jumail Mundekkat / MxBlue
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * EEPROM management code from ../cannonkeys/stm32f072/keyboard.c
 */

#include QMK_KEYBOARD_H
#include "tmk_core/common/eeprom.h"
#include "tmk_core/common/action_layer.h"
#include "rgblight.h"
#include "via.h"
#include "version.h" // for QMK_BUILDDATE used in EEPROM magic

void via_init_kb(void) {
	fled_init();
}

void matrix_init_kb(void) {
    // If VIA is disabled, we still need to load settings
	// Call via_init_kb() the same way as via_init(), with setting
	// EEPROM valid afterwards.
#ifndef VIA_ENABLE
    fled_init();
    via_eeprom_set_valid(true);
#endif // VIA_ENABLE

	matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Handle custom keycodes for front LED operation
    process_record_fled(keycode, record);
	return process_record_user(keycode, record);
}

bool led_update_kb(led_t led_state) {
    fled_lock_update(led_state);
	return led_update_user(led_state);
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    fled_layer_update(state);
    return layer_state_set_user(state);
}

// VIA specific
#ifndef VIA_ENABLE

#include "mxss_via_rgblight.h"

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
	uint8_t *command_data = &(data[1]);

	switch (*command_id) {
        case id_backlight_config_set_value:
            mxss_rgblight_set_value(command_data);
            break;
        case id_backlight_config_get_value:
            mxss_rgblight_get_value(command_data);
            break;
        case id_backlight_config_save:
            mxss_rgblight_save_value();
            break;
    }
}

// Sets VIA/keyboard level usage of EEPROM to valid/invalid
// Keyboard level code (eg. via_init_kb()) should not call this
void via_eeprom_set_valid(bool valid)
{
    char *p = QMK_BUILDDATE; // e.g. "2019-11-05-11:29:54"
    uint8_t magic0 = ( ( p[2] & 0x0F ) << 4 ) | ( p[3]  & 0x0F );
    uint8_t magic1 = ( ( p[5] & 0x0F ) << 4 ) | ( p[6]  & 0x0F );
    uint8_t magic2 = ( ( p[8] & 0x0F ) << 4 ) | ( p[9]  & 0x0F );

    eeprom_update_byte( (void*)VIA_EEPROM_MAGIC_ADDR+0, valid ? magic0 : 0xFF);
    eeprom_update_byte( (void*)VIA_EEPROM_MAGIC_ADDR+1, valid ? magic1 : 0xFF);
    eeprom_update_byte( (void*)VIA_EEPROM_MAGIC_ADDR+2, valid ? magic2 : 0xFF);
}

#endif
