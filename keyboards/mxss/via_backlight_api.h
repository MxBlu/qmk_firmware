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
enum BacklightConfigMessage {
  VIA_API_USE_SPLIT_BACKSPACE = 0x01,
  VIA_API_USE_SPLIT_LEFT_SHIFT = 0x02,
  VIA_API_USE_SPLIT_RIGHT_SHIFT = 0x03,
  VIA_API_USE_7U_SPACEBAR = 0x04,
  VIA_API_USE_ISO_ENTER = 0x05,
  VIA_API_DISABLE_HHKB_BLOCKER_LEDS = 0x06,
  VIA_API_DISABLE_WHEN_USB_SUSPENDED = 0x07,
  VIA_API_DISABLE_AFTER_TIMEOUT = 0x08,
  VIA_API_BRIGHTNESS = 0x09,
  VIA_API_EFFECT = 0x0a,
  VIA_API_EFFECT_SPEED = 0x0b,
  VIA_API_COLOR_1 = 0x0c,
  VIA_API_COLOR_2 = 0x0d,
  VIA_API_CAPS_LOCK_INDICATOR_COLOR = 0x0e,
  VIA_API_CAPS_LOCK_INDICATOR_ROW_COL = 0x0f,
  VIA_API_LAYER_1_INDICATOR_COLOR = 0x10,
  VIA_API_LAYER_1_INDICATOR_ROW_COL = 0x11,
  VIA_API_LAYER_2_INDICATOR_COLOR = 0x12,
  VIA_API_LAYER_2_INDICATOR_ROW_COL = 0x13,
  VIA_API_LAYER_3_INDICATOR_COLOR = 0x14,
  VIA_API_LAYER_3_INDICATOR_ROW_COL = 0x15,
  VIA_API_CUSTOM_COLOR = 0x17
};
