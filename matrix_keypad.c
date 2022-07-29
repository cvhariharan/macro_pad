#include "pico/stdlib.h"
#include <stdlib.h>
#include "matrix_keypad.h"

/*
* keypad_init initializes the keypad and returns a Keypad struct.
* @param col_pins GPIO pins connected to columns
* @param row_pins GPIO pins connected to rows
* @param keymap 4x4 2D array for characters
* @return Keypad
*/
Keypad keypad_init(int col_pins[4], int row_pins[4], char keymap[4][4]) {
    Keypad keypad;
    keypad.col_pins = (int *)malloc(4 * sizeof(int));
    keypad.row_pins = (int *)malloc(4 * sizeof(int));

    keypad.keymap = (char **)malloc(4 * sizeof(char *));
    for(int i = 0; i < 4; i++) {
        keypad.keymap[i] = (char *)malloc(4 * sizeof(char));
        keypad.keymap[i] = keymap[i];
    }

    keypad.col_pins = col_pins;
    keypad.row_pins = row_pins;
    keypad.col_mask = 0;

    for(int i = 0; i < 4; i++) {
        gpio_init(keypad.col_pins[i]);
        gpio_pull_down(keypad.col_pins[i]);
        gpio_set_dir(keypad.col_pins[i], GPIO_IN);

        keypad.col_mask = keypad.col_mask + (1 << col_pins[i]);

        gpio_init(keypad.row_pins[i]);
        gpio_set_dir(keypad.row_pins[i], GPIO_OUT);
        
    }
    keypad.last_pressed = get_absolute_time();

    return keypad;
}

/*
* key_pressed scans the keypad once to return the key. 
* Should be run inside an infinite loop
* @param Keypad keypad
*/
char keypad_key_pressed(Keypad *keypad) {
    uint32_t cols, i;
    for(i = 0; i < 4; i++) {
        gpio_put(keypad->row_pins[i], true);
        busy_wait_us(20);
        cols = gpio_get_all() & keypad->col_mask;
        if(cols != 0) {
            if(absolute_time_diff_us(keypad->last_pressed, get_absolute_time()) > 200000) {
                keypad->last_pressed = get_absolute_time();
                gpio_put(keypad->row_pins[i], false);
                break;
            }
            // keypad->last_pressed = get_absolute_time();
        }
        busy_wait_us(20);
        gpio_put(keypad->row_pins[i], false);
    }
    if(cols == (1 << keypad->col_pins[0])) {
        return keypad->keymap[i][0];
    } else if (cols == (1 << keypad->col_pins[1])) {
        return keypad->keymap[i][1];
    } else if (cols == (1 << keypad->col_pins[2])) {
        return keypad->keymap[i][2];
    } else if (cols == (1 << keypad->col_pins[3])) {
        return keypad->keymap[i][3];
    }
    return '\0';
}

/*
* keypad_destroy releases the allocated memory
* @param Keypad keypad
*/
void keypad_destroy(Keypad keypad) {
    free(keypad.col_pins);
    free(keypad.row_pins);
    for(int i = 0; i < 4; i++) {
        free(keypad.keymap[i]);
    }
    free(keypad.keymap);
}

