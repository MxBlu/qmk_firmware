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
 */

#include "quantum.h"
#include "rgblight.h"
#include "mxss_via_rgblight.h"
#include "via_backlight_api.h"
#include "mxss_frontled.h"

void mxss_rgblight_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_qmk_backlight_effect:
            value_data[0] = get_fled_mode();
            break;
        case id_qmk_backlight_brightness:
            value_data[0] = get_fled_val();
            break;
        case id_backlight_caps_lock_indicator_color:
            *((hs_set *)value_data) = get_fled_caps_color();
            break;
        case id_backlight_layer_1_indicator_color:
            *((hs_set *)value_data) = get_fled_layer_color(1);
            break;
        case id_backlight_layer_2_indicator_color:
            *((hs_set *)value_data) = get_fled_layer_color(2);
            break;
        case id_backlight_layer_3_indicator_color:
            *((hs_set *)value_data) = get_fled_layer_color(3);
            break;
    }
}

void mxss_rgblight_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_qmk_backlight_effect:
            set_fled_mode(value_data[0]);
            break;
        case id_qmk_backlight_brightness:
            set_fled_val(value_data[0]);
            break;
        case id_backlight_caps_lock_indicator_color:
            set_fled_caps_color(*(hs_set *)value_data);
            break;
        case id_backlight_layer_1_indicator_color:
            set_fled_layer_color(1, *(hs_set *)value_data);
            break;
        case id_backlight_layer_2_indicator_color:
            set_fled_layer_color(2, *(hs_set *)value_data);
            break;
        case id_backlight_layer_3_indicator_color:
            set_fled_layer_color(3, *(hs_set *)value_data);
            break;
    }
}

void mxss_rgblight_save_value() {
    fled_update_conf();
}
