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

#pragma once

// Retrieved from https://github.com/the-via/reader/blob/master/src/types.ts
enum via_lighting_value_ext {
  id_qmk_rgblight_caps_lock_indicator_color = 0x0e,
  id_qmk_rgblight_layer_1_indicator_color = 0x10,
  id_qmk_rgblight_layer_2_indicator_color = 0x12,
  id_qmk_rgblight_layer_3_indicator_color = 0x14,
};
