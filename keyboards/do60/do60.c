#include "do60.h"
#include "led.h"

// Sennin's crap, lets just ignore it

//extern inline void do60_caps_led_on(void);
//extern inline void do60_bl_led_on(void);

//extern inline void do60_caps_led_off(void);
//extern inline void do60_bl_led_off(void);

//extern inline void setdefaultrgb(void);

// Mx's beautiful code
#include "mx_layer.h"

void matrix_init_kb(void) {
  // Keyboard start-up code goes here
  // Runs once when the firmware starts up
  matrix_init_user();
  led_init_ports();
  //setdefaultrgb();

  rgbl_init();
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
  rgbl_layer_update(state);
  return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // Handle custom keycodes for LED operation
  rgbl_process(keycode, record);

  switch (keycode) {
    case KC_C: // Win + C = KC_CALC
    if (record->event.pressed) {
      if (keyboard_report->mods & MOD_BIT(KC_LGUI)) {
        unregister_code(KC_LGUI);
        register_code(KC_CALC);
        return false;
      }
    } else {
      unregister_code(KC_CALC);
    }
    break;

    default:
    break; // Process all other keycodes normally
  }

  return process_record_user(keycode, record);
}
