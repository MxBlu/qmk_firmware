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
 * Extended from the work done by fcoury: https://github.com/qmk/qmk_firmware/pull/4915
 */

#include "hbcp_frontled.h"
#include "tmk_core/common/eeprom.h"
#include "rgblight.h"
#include "via.h"
#include "version.h" // for QMK_BUILDDATE used in EEPROM magic

// Variables for controlling front LED application
uint8_t fled_mode;  // Mode for front LEDs
uint8_t fled_val;   // Brightness for front leds (0 - 255)
LED_TYPE fleds[3];  // Front LED rgb values for indicator mode use

// Indicator colors
__attribute__ ((weak))
hs_set indicator_colors[FRONTLED_COLOR_MAXCNT];

static void set_lock_or_black(uint8_t, uint8_t, hs_set, RGB*);

void fled_init(void) {
	// If EEPROM config exists, load it
	// If VIA EEPROM exists, FLED config should too
	if (via_eeprom_is_valid()) {
		fled_load_conf();
	// Else, default config
	} else {
		// Default mode/brightness
		fled_mode = FLED_RGB;
		fled_val = 10 * FLED_VAL_STEP;

		// Default colors
		for (int i = 0; i < 3; i++) {
			indicator_colors[0].hue = 255;
			indicator_colors[0].sat = 255;
		}

		fled_update_conf();   // Store default config to EEPROM
	}

	// Set default values for leds
	setrgb(0, 0, 0, &fleds[0]);
	setrgb(0, 0, 0, &fleds[1]);
	setrgb(0, 0, 0, &fleds[2]);

	// Handle lighting for indicator mode
	if (fled_mode == FLED_INDI) {
		fled_lock_update(host_keyboard_leds());
	}
}

void  process_record_fled(uint16_t keycode, keyrecord_t *record) {
	// Handle custom keycodes for front LED operation
	switch (keycode) {
		case FLED_MOD: // Change between front LED operation modes (off, indicator, RGB)
		if (record->event.pressed)
			fled_mode_cycle();
		break;

		case FLED_VAI: // Increase the brightness of the front LEDs by FLED_VAL_STEP
		if (record->event.pressed)
			fled_val_increase();
		break;

		case FLED_VAD: // Decrease the brightness of the front LEDs by FLED_VAL_STEP
		if (record->event.pressed)
			fled_val_decrease();
		break;

		default:
		break; // Process all other keycodes normally
	  }

	return;
}

void fled_load_conf(void) {
	// Load config
	fled_config fled_conf;
	fled_conf.raw = eeprom_read_byte(FRONTLED_CONF_ADDR);
	fled_mode = fled_conf.mode;
	fled_val = fled_conf.val * FLED_VAL_STEP;

	// Load color data
	uint16_t *color_ptr = FRONTLED_COLOR_ADDR;
	for (uint8_t i = 0; i < FRONTLED_COLOR_MAXCNT; i++) {
		indicator_colors[i].raw = eeprom_read_word(&color_ptr[i]);
	}
}

// Store current front led config in EEPROM
void fled_update_conf(void)
{
	// Create storage struct and set values
	fled_config conf;
	conf.mode = fled_mode;

	// Small hack to ensure max value is stored correctly
	if (fled_val == 255)
		conf.val = 256 / FLED_VAL_STEP;
	else
		conf.val = fled_val / FLED_VAL_STEP;

	// Store config
	eeprom_update_byte(FRONTLED_CONF_ADDR, conf.raw);

	// Store color data
	uint16_t *color_ptr = FRONTLED_COLOR_ADDR;
	for (uint8_t i = 0; i < FRONTLED_COLOR_MAXCNT; i++) {
		eeprom_update_word(&color_ptr[i], indicator_colors[i].raw);
	}
}

// Custom keycode functions

void fled_mode_cycle(void)
{
	// FLED -> FLED_RGB -> FLED_INDI
	switch (fled_mode) {
		case FLED_OFF:
		fled_mode = FLED_RGB;
		rgblight_timer_enable();
		break;

		case FLED_RGB:
		fled_mode = FLED_INDI;
		break;

		case FLED_INDI:
		fled_mode = FLED_OFF;
		break;
	}

	// Update stored config
	fled_update_conf();
	rgblight_set();
}

void fled_val_increase(void)
{
	// Increase val by FLED_VAL_STEP, handling the upper edge case
	if (fled_val + FLED_VAL_STEP > 255)
		fled_val = 255;
	else
		fled_val += FLED_VAL_STEP;

	// Update stored config
	fled_update_conf();
	rgblight_set();
}

void fled_val_decrease(void)
{
	// Decrease val by FLED_VAL_STEP, handling the lower edge case
	if (fled_val - FLED_VAL_STEP > 255)
		fled_val = 255;
	else
		fled_val -= FLED_VAL_STEP;

	// Update stored config
	fled_update_conf();
	rgblight_set();
}

void fled_lock_update(uint8_t usb_led) {
	// Set indicator LED appropriately, whether it is used or not
	for (uint8_t i = USB_LED_NUM_LOCK; i < USB_LED_SCROLL_LOCK; i++) {
		set_lock_or_black(usb_led, i, indicator_colors[i], &fleds[i]);
	}

	rgblight_set();
}

void set_fled_lock_color(uint8_t lock, hs_set hs) {
	// Update lock color and refresh LEDs
	indicator_colors[lock] = hs;
	fled_lock_update(host_keyboard_leds());
	fled_update_conf();
}

void set_fled_val(uint8_t val) {
	fled_val = val;
}

hs_set get_fled_lock_color(uint8_t lock) {
	return indicator_colors[lock];
}

uint8_t get_fled_val(void) {
	return fled_val;
}

static void set_lock_or_black(uint8_t usb_led, uint8_t lock, hs_set hs, RGB *led) {
	if (usb_led & (1 << lock))
		sethsv(hs.hue, hs.sat, fled_val, led);
	else
		setrgb(0, 0, 0, led);
}

// Fallback eeprom functions if VIA is not enabled
#ifndef VIA_ENABLE

// Can be called in an overriding via_init_kb() to test if keyboard level code usage of
// EEPROM is invalid and use/save defaults.
bool via_eeprom_is_valid(void)
{
	char *p = QMK_BUILDDATE; // e.g. "2019-11-05"
	uint8_t magic0 = ( ( p[2] & 0x0F ) << 4 ) | ( p[3]  & 0x0F );
	uint8_t magic1 = ( ( p[5] & 0x0F ) << 4 ) | ( p[6]  & 0x0F );
	uint8_t magic2 = ( ( p[8] & 0x0F ) << 4 ) | ( p[9]  & 0x0F );

	return (eeprom_read_byte( (void*)VIA_EEPROM_MAGIC_ADDR+0 ) == magic0 &&
			eeprom_read_byte( (void*)VIA_EEPROM_MAGIC_ADDR+1 ) == magic1 &&
			eeprom_read_byte( (void*)VIA_EEPROM_MAGIC_ADDR+2 ) == magic2 );
}

#endif
