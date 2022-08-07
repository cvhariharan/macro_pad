#include <stdio.h>
#include "pico/stdlib.h"
#include "matrix_keypad.h"
#include "bsp/board.h"

#include "usb_descriptors.h"

static char keymap[3][3] = {
                        {'7','4','1'},
                        {'8','5','2'},
                        {'9','6','3'}
                    };
static int col_pins[3] = {10,4,3};
static int row_pins[3] = {9,8,7};

static Keypad keypad;

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

void send_hid_report(uint8_t report_id, char btn)
{
  // Skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

    if(report_id == REPORT_ID_KEYBOARD) {
        static bool has_keyboard_key = false;
        if ( !has_keyboard_key )
        {
          uint8_t keycode[6] = { 0 };
          switch(btn) {
            case '1':
                keycode[0] = HID_KEY_A;

                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
                has_keyboard_key = true;
            break;
            case '4':
                keycode[0] = HID_KEY_B;

                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
                has_keyboard_key = true;
            break;
          }
        }
        else
        {
          // Send empty key report if previously has key pressed
          if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
          has_keyboard_key = false;
        }
    }
}

void hid_task(void) {
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  char btn = keypad_key_pressed(&keypad);
  // printf("%d - %c\n", start_ms, btn);

  if (tud_suspended() && btn != '\0')
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }else
  {
    // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
    send_hid_report(REPORT_ID_KEYBOARD, btn);
  }
}


void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}


void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
  (void) instance;
  (void) len;
}