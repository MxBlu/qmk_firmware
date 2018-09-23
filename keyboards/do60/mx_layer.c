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

#include "mx_layer.h"
#include "rgblight.h"
#include "quantum.h"

rgblayer_config rconf;
bool rgblayer_pmode;
uint8_t pmode_layer;

static void rgbl_setlayer(uint8_t layer);

void rgbl_init(void) {
  // If EEPROM config exists, load it
  if (eeprom_is_valid()) {
    eeprom_read_conf();
  // Else, default config
  } else {
    eeprom_default_conf();
  }

  rgblayer_pmode = false;
  pmode_layer = 0;
};

void rgbl_layer_update(uint32_t state) {\
  if (rgblight_get_mode() == RGBLIGHT_MODE_RGB_LAYER && !rgblayer_pmode) {
      uint8_t layer = biton32(state);
      rgbl_setlayer(layer);
      rgblight_set();
  }
}

void rgbl_process(uint16_t keycode, keyrecord_t *record) {
  if (rgblight_get_mode() == RGBLIGHT_MODE_RGB_LAYER) {
    switch (keycode) {
      case RGBL_PMODE:
      if (record->event.pressed)
        rgbl_pmode_toggle();
      break;

      case RGBL_CYC:
      if (record->event.pressed)
        rgbl_pmode_cycle();
      break;

      default:
      break; // Process all other keycodes normally
    }
  }
}

// RGB Layer Code

static void rgbl_setlayer(uint8_t layer) {
  if (layer < rconf.num_layers)
    rgblight_sethsv_noeeprom(rconf.layers[layer].hue, rconf.layers[layer].sat, rconf.layers[layer].val);
  else
    rgblight_sethsv_noeeprom(0, 0, 0);
}

void rgbl_pmode_toggle() {
  rgblayer_pmode = !rgblayer_pmode;
  pmode_layer = 0;

  if (rgblayer_pmode)
    rgbl_setlayer(pmode_layer);
  else
    rgblight_mode(RGBLIGHT_MODE_RGB_LAYER);
}

void rgbl_pmode_cycle() {
  if (rgblayer_pmode) {
    pmode_layer = (pmode_layer + 1) % _NUM_LAYERS;
    rgbl_setlayer(pmode_layer);
  }
}

// EEPROM Management

// Test if magic value is present at expected location
bool eeprom_is_valid(void)
{
	return (eeprom_read_word(EEPROM_MAGIC_ADDR) == EEPROM_MAGIC && eeprom_read_byte (EEPROM_NUMLAYER_ADDR) == _NUM_LAYERS);
}

// Reset EEPROM config
void eeprom_default_conf(void)
{
	eeprom_update_word(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);

    rconf.num_layers = _NUM_LAYERS;
    for (int i = 0; i < _NUM_LAYERS; i++)
        rconf.layers[i].hue = rconf.layers[i].sat = rconf.layers[i].val = 0;

    eeprom_update_conf();
}

// Store current config in EEPROM
void eeprom_update_conf(void)
{
    eeprom_update_byte(EEPROM_NUMLAYER_ADDR, rconf.num_layers);
    for (int i = 0; i < _NUM_LAYERS; i++)
        eeprom_update_dword(EEPROM_LAYERS_ADDR + i*32, rconf.layers[i].raw);
}

// Store current front led config in EEPROM
void eeprom_read_conf(void)
{
    rconf.num_layers = eeprom_read_byte(EEPROM_NUMLAYER_ADDR);
    if (rconf.num_layers != _NUM_LAYERS)
        eeprom_default_conf();

    for (int i = 0; i < _NUM_LAYERS; i++)
        rconf.layers[i].raw = eeprom_read_dword(EEPROM_LAYERS_ADDR + i*32);
}
