/* Copyright 2018 Jumail Mundekkat / MxBlue
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

#include "mxss.h"
#include "mx_layer.h"

void matrix_init_kb(void) {
  // Keyboard start-up code goes here
  // Runs once when the firmware starts up
  matrix_init_user();

  rgbl_init();
};

void matrix_scan_kb(void) {
  // Looping keyboard code goes here
  // This runs every cycle (a lot)
  matrix_scan_user();
};

uint32_t layer_state_set_kb(uint32_t state) {\
  rgbl_layer_update(state);
  return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // Handle custom keycodes for LED operation
  rgbl_process(keycode, record);

  switch (keycode) {
    default:
    break; // Process all other keycodes normally
  }

  return process_record_user(keycode, record);
}
