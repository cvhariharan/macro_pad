typedef struct Keypad4x4 {
    int* col_pins;
    int* row_pins;
    char** keymap;
    int col_mask;
    absolute_time_t last_pressed;
    uint8_t size;
} Keypad;

Keypad keypad_init(uint8_t size, int *, int *, char [size][size]);
char keypad_key_pressed(Keypad*);
void keypad_destroy(Keypad);