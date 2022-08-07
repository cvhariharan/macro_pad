#include "pico/stdlib.h"
#include <stdlib.h>
#include "matrix_keypad.h"

Keypad keypad_init(uint8_t size, int *col_pins, int *row_pins, char keymap[size][size]) {
    Keypad keypad;

    keypad.keymap = (char **)malloc(size * sizeof(char *));
    for(int i = 0; i < size; i++) {
        keypad.keymap[i] = (char *)malloc(size * sizeof(char));
        keypad.keymap[i] = keymap[i];
    }

    printf("Keymap size %d\n", sizeof(keypad.keymap));

    keypad.col_pins = col_pins;
    keypad.row_pins = row_pins;
    keypad.col_mask = 0;

    keypad.size = size;

    for(int i = 0; i < size; i++) {
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

char keypad_key_pressed(Keypad *keypad) {
    uint32_t cols, i;
    for(i = 0; i < keypad->size; i++) {
        gpio_put(keypad->row_pins[i], true);
        busy_wait_us(20);
        cols = gpio_get_all() & keypad->col_mask;
        if(cols != 0) {
            if(absolute_time_diff_us(keypad->last_pressed, get_absolute_time()) > 200000) {
                keypad->last_pressed = get_absolute_time();
                gpio_put(keypad->row_pins[i], false);
                break;
            }
        }
        busy_wait_us(20);
        gpio_put(keypad->row_pins[i], false);
    }

    for(int j = 0; j < keypad->size; j++) {
        if(cols == (1 << keypad->col_pins[j])) {
            return keypad->keymap[i][j];
        }
    }
    return '\0';
}

void keypad_destroy(Keypad keypad) {
    free(keypad.col_pins);
    free(keypad.row_pins);
    for(int i = 0; i < keypad.size; i++) {
        free(keypad.keymap[i]);
    }
    free(keypad.keymap);
}

