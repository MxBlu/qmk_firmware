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

#include "quantum.h"
#include "rgblight.h"
#include "rgb_backlight.h"
#include "via_backlight_api.h"
#include "hbcp_frontled.h"
#include "debug.h"

// Cache this, so we can reuse it when we change modes
uint8_t effect_speed = 0;

// Returns the maximum speed provided by the modes for a given base mode
uint8_t get_max_effect_speed(uint8_t mode) {
	switch (mode) {
		case RGBLIGHT_MODE_SNAKE: // Actually 2, but 4 because odd values are reversed direction
			return 4;
		case RGBLIGHT_MODE_BREATHING:
			return 3;
		case RGBLIGHT_MODE_RAINBOW_MOOD:
		case RGBLIGHT_MODE_RAINBOW_SWIRL:
		case RGBLIGHT_MODE_KNIGHT:
			return 2;
		case RGBLIGHT_MODE_STATIC_LIGHT:
		case RGBLIGHT_MODE_CHRISTMAS:
		case RGBLIGHT_MODE_STATIC_GRADIENT:
		case RGBLIGHT_MODE_RGB_TEST:
		case RGBLIGHT_MODE_ALTERNATING:
		default:
			return 0;
	}
}

void set_effect_speed(uint8_t data) {
	effect_speed = data;
	uint8_t mode = rgblight_get_base_mode();
	uint8_t speed_max = get_max_effect_speed(mode);
	if (mode == RGBLIGHT_MODE_SNAKE) // Snake is special
		effect_speed *= 2; // Odd values are reversed snaking, even are default

	// Variations on the base mode include speed and direction
	// speed_max assists us to make sure we don't end up on the next mode
	mode += effect_speed > speed_max ? speed_max : effect_speed;
	rgblight_mode(mode);
}

void set_effect(uint8_t *value_data) {
	switch (*value_data) {
		case 0:
			rgblight_disable();
			break;
		case 1:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
			break;
		case 2:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_BREATHING);
			break;
		case 3:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
			break;
		case 4:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
			break;
		case 5:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_SNAKE);
			break;
		case 6:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_KNIGHT);
			break;
		case 7:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_CHRISTMAS);
			break;
		case 8:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_STATIC_GRADIENT);
			break;
		case 9:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_RGB_TEST);
			break;
		case 10:
			rgblight_enable();
			rgblight_mode(RGBLIGHT_MODE_ALTERNATING);
			break;
	}
	// Ensure that we respect the speed that VIA knows
	set_effect_speed(effect_speed);
}

void set_color(uint8_t *data) {
	uint8_t *hue = &(data[0]);
	uint8_t *sat = &(data[1]);
	rgblight_sethsv(*hue, *sat, rgblight_get_val());
}

void set_brightness(uint8_t *data) {
	uint8_t hue = rgblight_get_hue();
	uint8_t sat = rgblight_get_sat();
	rgblight_sethsv(hue, sat, *data);
}

void set_lock_color(uint8_t lock, uint8_t *data) {
	hs_set *hs = (hs_set *)data;
	set_fled_lock_color(lock, *hs);
}

void get_color(uint8_t *data) {
	uint8_t hue = rgblight_get_hue();
	uint8_t sat = rgblight_get_sat();
	data[0]     = hue;
	data[1]     = sat;
}

uint8_t get_effect(void) {
	uint8_t mode = rgblight_get_base_mode();
	switch (mode) {
		case RGBLIGHT_MODE_STATIC_LIGHT:
			return 1;
		case RGBLIGHT_MODE_BREATHING:
			return 2;
		case RGBLIGHT_MODE_RAINBOW_MOOD:
			return 3;
		case RGBLIGHT_MODE_RAINBOW_SWIRL:
			return 4;
		case RGBLIGHT_MODE_SNAKE:
			return 5;
		case RGBLIGHT_MODE_KNIGHT:
			return 6;
		case RGBLIGHT_MODE_CHRISTMAS:
			return 7;
		case RGBLIGHT_MODE_STATIC_GRADIENT:
			return 8;
		case RGBLIGHT_MODE_RGB_TEST:
			return 9;
		case RGBLIGHT_MODE_ALTERNATING:
			return 10;
		default:
			return 0;
	}
}

void get_effect_speed(uint8_t *data) {
	uint8_t mode = rgblight_get_base_mode();
	uint8_t speed_max = get_max_effect_speed(mode);
	effect_speed = rgblight_get_mode() - mode; // See set_effect_speed for logic
	*data = effect_speed > speed_max ? speed_max : effect_speed;
}

void backlight_config_get_value(uint8_t *data) {
	uint8_t *value_id   = &(data[0]);
	uint8_t *value_data = &(data[1]);
	switch (*value_id) {
		case VIA_API_EFFECT:
			// Return [ effect_id (variable, 0-10 for qmk_underglow) ]
			*value_data = rgblight_get_base_mode();
			break;
		case VIA_API_EFFECT_SPEED:
			// Return [ effect_speed (0-3) ]
			get_effect_speed(value_data);
			break;
		case VIA_API_BRIGHTNESS:
			// Return [ val (0-255) ]
			*value_data = rgblight_get_val();
			break;
		case VIA_API_COLOR_1:
			// Return [ hue(0-255), sat(0-255) ]
			get_color(value_data);
			break;
		case VIA_API_LAYER_1_INDICATOR_COLOR:
			// Return [ hue(0-255), sat(0-255) ]
			*((hs_set *)value_data) = get_fled_lock_color(0);
			break;
		case VIA_API_LAYER_2_INDICATOR_COLOR:
			// Return [ hue(0-255), sat(0-255) ]
			*((hs_set *)value_data) = get_fled_lock_color(1);
			break;
		case VIA_API_LAYER_3_INDICATOR_COLOR:
			// Return [ hue(0-255), sat(0-255) ]
			*((hs_set *)value_data) = get_fled_lock_color(2);
			break;
	}
}

void backlight_config_set_value(uint8_t *data) {
	uint8_t *value_id   = &(data[0]);
	uint8_t *value_data = &(data[1]);
	switch (*value_id) {
		case VIA_API_EFFECT:
			// Set [ effect_id (variable, 0-10 for qmk_underglow) ]
			set_effect(value_data);
			break;
		case VIA_API_EFFECT_SPEED:
			// Set [ effect_speed (0-3) ]
			set_effect_speed(*value_data);
			break;
		case VIA_API_BRIGHTNESS:
			// Set [ val (0-255) ]
			set_brightness(value_data);
			break;
		case VIA_API_COLOR_1:
			// Set [ hue(0-255), sat(0-255) ]
			set_color(value_data);
			break;
		case VIA_API_LAYER_1_INDICATOR_COLOR:
			// Set [ hue(0-255), sat(0-255) ]
			set_lock_color(0, value_data);
			break;
		case VIA_API_LAYER_2_INDICATOR_COLOR:
			// Set [ hue(0-255), sat(0-255) ]
			set_lock_color(1, value_data);
			break;
		case VIA_API_LAYER_3_INDICATOR_COLOR:
			// Set [ hue(0-255), sat(0-255) ]
			set_lock_color(2, value_data);
			break;
	}
}
