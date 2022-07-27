#include <stdio.h>
#include "pico/stdlib.h"
#include "matrix_keypad.h"

void main() {
    stdio_init_all();
    busy_wait_ms(2000);

    char keymap[4][4] = {{'1','2','3','A'},
                        {'4','5','6','B'},
                        {'7','8','9','C'},
                        {'*','0','#','D'}};

                    // c1c2c3c4
    int col_pins[4] = {10,4,3,2};
                    // r1r2r3r4
    int row_pins[4] = {9,8,7,6};

    Keypad keypad = keypad_init(col_pins, row_pins, keymap);

    while(1) {
        char c = keypad_key_pressed(&keypad);
        if(c != '\0') {
            printf("%c", c);
        }
        // busy_wait_ms(100);
    }
    //keypad_destroy(keypad);
}
