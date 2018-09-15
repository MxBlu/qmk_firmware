#include "do60.h"
#include "led.h"

// Sennin's crap, lets just ignore it

//extern inline void do60_caps_led_on(void);
//extern inline void do60_bl_led_on(void);

//extern inline void do60_caps_led_off(void);
//extern inline void do60_bl_led_off(void);

//extern inline void setdefaultrgb(void);

// Mx's beautiful code

#include "rgblight.h"
#include "mx_layer.h"

extern rgblight_config_t rgblight_config;

rgblayer_config rconf;
bool rgblayer_pmode;
uint8_t pmode_layer;

uint16_t shft_pressed = 0;
bool bkspc_del = false;

void matrix_init_kb(void) {
  // Keyboard start-up code goes here
  // Runs once when the firmware starts up
  matrix_init_user();
  led_init_ports();
  //setdefaultrgb();
  
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

void matrix_scan_kb(void) {
  // Looping keyboard code goes here
  // This runs every cycle (a lot)
  matrix_scan_user();
};

void led_init_ports(void) {
  // Set caps lock LED pin as output
  DDRB |= (1 << 2);
  // Default to off
  PORTB |= (1 << 2);
}

void led_set_kb(uint8_t usb_led) {
	//if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
	//	do60_caps_led_on();
	//} else {
	//	do60_caps_led_off();
	//}

	//led_set_user(usb_led);

	if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        // Turn capslock on
        PORTB &= ~(1<<2);
    } else {
        // Turn capslock off
        PORTB |= (1<<2);
    }
}

uint32_t layer_state_set_kb(uint32_t state) {
    
    if (rgblight_config.mode == 37 && !rgblayer_pmode) {
        uint8_t layer = biton32(state);
        rgblayer_setlayer(layer);
    }
    
    return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Handle custom keycodes for LED operation
    switch (keycode) {
        case RGBL_PMODE: // Change between front LED operation modes (off, indicator, RGB)
        if (record->event.pressed)
            rgblayer_pmode_toggle();
        break;
        
        case RGBL_CYC: // Increase the brightness of the front LEDs by FLED_VAL_STEP
        if (record->event.pressed)
            rgblayer_pmode_cycle();
        break;
        
        case KC_RSFT:        
        case KC_LSFT:
        if (record->event.pressed) {
            shft_pressed = keycode;
        } else {
            shft_pressed = 0;
            if (bkspc_del) {
                bkspc_del = false;
                unregister_code(KC_DEL);
                register_code(KC_BSPC);
            }
            unregister_code(keycode);
            return false;
        }
        
        break;
        
        case BS_DEL:
        if (record->event.pressed) {
            if (shft_pressed) {
                bkspc_del = true;
                unregister_code(shft_pressed);
                register_code(KC_DEL);
            } else {
                register_code(KC_BSPC);
            }
        } else {
            if (bkspc_del) {
                bkspc_del = false;
                unregister_code(KC_DEL);
                register_code(shft_pressed);
            }
            return false;
        }
        
        break;
        
        default:
        break; // Process all other keycodes normally
      }

    return process_record_user(keycode, record);
}

// RGB Layer Code

void rgblayer_setlayer(uint8_t layer) {
    if (layer < rconf.num_layers)
        rgblight_sethsv(rconf.layers[layer].hue, rconf.layers[layer].sat, rconf.layers[layer].val);
    else
        rgblight_sethsv(0, 0, 0);
}

void rgblayer_pmode_toggle() {
    rgblayer_pmode = !rgblayer_pmode;
    pmode_layer = 0;
    
    if (rgblayer_pmode)
        rgblayer_setlayer(0);
    else
        rgblight_mode(37);
}

void rgblayer_pmode_cycle() {
    if (rgblayer_pmode) {
        pmode_layer = (pmode_layer + 1) % _NUM_LAYERS;
        rgblayer_setlayer(pmode_layer);
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
