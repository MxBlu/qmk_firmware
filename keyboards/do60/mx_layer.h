#ifndef MX_LAYER_H
#define MX_LAYER_H

#include "quantum_keycodes.h"
#include "action.h"
#include <avr/eeprom.h>

// QMK never uses more then 32bytes of EEPROM, so our region starts there
// Magic value to verify the state of the EEPROM
#define EEPROM_MAGIC 0xB3DA
#define EEPROM_MAGIC_ADDR ((void*)32)
#define _NUM_LAYERS 4 // Expected number of layers to be defined

// Front LED settings
#define EEPROM_NUMLAYER_ADDR ((void*)34)
#define EEPROM_LAYERS_ADDR ((void*)37)

typedef union {
    uint32_t raw;
    struct {
        uint16_t hue;
        uint8_t sat;
        uint8_t val;
    };
} hsv_set;

// Config storage format for EEPROM
typedef struct {
    uint8_t  num_layers;
    hsv_set layers[_NUM_LAYERS];
} rgblayer_config;

enum rgbl_keycodes {
  RGBL_PMODE = SAFE_RANGE,
  RGBL_CYC,
  NEW_SAFE_RANGE // define a new safe range
};

void rgbl_init(void);
void rgbl_layer_update(uint32_t state);
void rgbl_process(uint16_t keycode, keyrecord_t *record);

bool eeprom_is_valid(void);         // Check if EEPROM has been set up
void eeprom_default_conf(void);                 // Change validity state of EEPROM
void eeprom_update_conf(void);      // Store current front LED config to EEPROM
void eeprom_read_conf(void);

void rgbl_pmode_toggle(void);
void rgbl_pmode_cycle(void);

#endif
