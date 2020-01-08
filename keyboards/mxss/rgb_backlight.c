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
#include "mxss_frontled.h"
#include "debug.h"

void set_effect(uint8_t *value_data) {
    switch (*value_data) {
        case 0:
            rgblight_disable();
            break;
        case 1:
            rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_enable();
            break;
        case 2:
            rgblight_mode(RGBLIGHT_MODE_BREATHING);
            rgblight_enable();
            break;
        case 3:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
            rgblight_enable();
            break;
        case 4:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
            rgblight_enable();
            break;
        case 5:
            rgblight_mode(RGBLIGHT_MODE_SNAKE);
            rgblight_enable();
            break;
        case 6:
            rgblight_mode(RGBLIGHT_MODE_KNIGHT);
            rgblight_enable();
            break;
        case 7:
            rgblight_mode(RGBLIGHT_MODE_CHRISTMAS);
            rgblight_enable();
            break;
        case 8:
            rgblight_mode(RGBLIGHT_MODE_STATIC_GRADIENT);
            rgblight_enable();
            break;
        case 9:
            rgblight_mode(RGBLIGHT_MODE_RGB_TEST);
            rgblight_enable();
            break;
        case 10:
            rgblight_mode(RGBLIGHT_MODE_ALTERNATING);
            rgblight_enable();
            break;
    }
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

void set_layer_color(uint8_t layer, uint8_t *data) {
    hs_set *hs = (hs_set *)data;
    set_fled_layer_color(layer, *hs);
}

void set_caps_color(uint8_t *data) {
    hs_set *hs = (hs_set *)data;
    set_fled_caps_color(*hs);
}

void get_color(uint8_t *data) {
    uint8_t hue = rgblight_get_hue();
    uint8_t sat = rgblight_get_sat();
    data[0]     = hue;
    data[1]     = sat;
}

uint8_t get_effect(void) {
    uint8_t mode = rgblight_get_mode();
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

void backlight_config_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case VIA_API_EFFECT:
            *value_data = get_effect();
            break;
        case VIA_API_EFFECT_SPEED:
            xprintf("getval\n");
            break;
        case VIA_API_BRIGHTNESS:
            *value_data = rgblight_get_val();
            break;
        case VIA_API_COLOR_1:
            get_color(value_data);
            break;
        case VIA_API_CAPS_LOCK_INDICATOR_COLOR:
            *((hs_set *)value_data) = get_fled_caps_color();
            break;
        case VIA_API_LAYER_1_INDICATOR_COLOR:
            *((hs_set *)value_data) = get_fled_layer_color(0);
            break;
        case VIA_API_LAYER_2_INDICATOR_COLOR:
            *((hs_set *)value_data) = get_fled_layer_color(1);
            break;
        case VIA_API_LAYER_3_INDICATOR_COLOR:
            *((hs_set *)value_data) = get_fled_layer_color(2);
            break;
    }
}

void backlight_config_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case VIA_API_EFFECT:
            set_effect(value_data);
            break;
        case VIA_API_EFFECT_SPEED:
            xprintf("setval: %u\n", *value_data);
            break;
        case VIA_API_BRIGHTNESS:
            set_brightness(value_data);
            break;
        case VIA_API_COLOR_1:
            set_color(value_data);
            break;
        case VIA_API_CAPS_LOCK_INDICATOR_COLOR:
            set_caps_color(value_data);
            break;
        case VIA_API_LAYER_1_INDICATOR_COLOR:
            set_layer_color(0, value_data);
            break;
        case VIA_API_LAYER_2_INDICATOR_COLOR:
            set_layer_color(1, value_data);
            break;
        case VIA_API_LAYER_3_INDICATOR_COLOR:
            set_layer_color(2, value_data);
            break;
    }
}
