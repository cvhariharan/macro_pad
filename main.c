#include <stdio.h>
#include "pico/stdlib.h"
#include "matrix_keypad.h"
#include "bsp/board.h"

#include "usb_descriptors.h"

static char keymap[3][3] = {{'1','4','7'},
                            {'2','5','8'},
                            {'3','6','9'}};
static int col_pins[3] = {10,4,3};
static int row_pins[3] = {9,8,7};

static Keypad keypad;

bool firefox = false;

int main() {
    board_init();
    tusb_init();

    stdio_init_all();
    busy_wait_ms(2000);
    keypad = keypad_init(3, col_pins, row_pins, keymap);

    while (1) {
        tud_task();
        hid_task();
    }

    keypad_destroy(keypad);
    return 0;
}


// send_hid_report can be modified with custom key bindings
void send_hid_report(uint8_t report_id, char btn) {
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;
  static bool has_keyboard_key = false;
  static bool is_playing = false;
  if(report_id == REPORT_ID_KEYBOARD) {
      if (!has_keyboard_key) {
        uint8_t keycode[6] = { 0 };
        switch(btn) {
          case '1':
              // copy selected text and launch firefox
              keycode[0] = HID_KEY_COPY;
              keycode[1] = HID_KEY_GUI_LEFT;
              keycode[2] = HID_KEY_F;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '2':
              // launch terminal
              keycode[0] = HID_KEY_CONTROL_LEFT;
              keycode[1] = HID_KEY_ALT_LEFT;
              keycode[2] = HID_KEY_T;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '3':
              // close current window
              keycode[0] = HID_KEY_ALT_LEFT;
              keycode[1] = HID_KEY_F4;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '4':
              // mute audio
              keycode[0] = HID_KEY_MUTE;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '5':
              // volume up
              keycode[0] = HID_KEY_VOLUME_UP;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '6':
              // volume down
              keycode[0] = HID_KEY_VOLUME_DOWN;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '7':
              // paste and enter
              keycode[0] = HID_KEY_PASTE;
              keycode[1] = HID_KEY_ENTER;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '8':
              // copy
              keycode[0] = HID_KEY_CONTROL_LEFT;
              keycode[1] = HID_KEY_C;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
          case '9':
              // paste
              keycode[0] = HID_KEY_CONTROL_LEFT;
              keycode[1] = HID_KEY_V;
              tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
              has_keyboard_key = true;
          break;
        }
      }
      else {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
  }
}

void hid_task(void) {
  // poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  char btn = keypad_key_pressed(&keypad);

  if (tud_suspended() && btn != '\0') {
    tud_remote_wakeup();
  }else {
    send_hid_report(REPORT_ID_KEYBOARD, btn);
  }
}


void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}


void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len) {
  (void) instance;
  (void) report;
  (void) len;
}