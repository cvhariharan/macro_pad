typedef struct Keypad4x4 {
    int* col_pins;
    int* row_pins;
    char** keymap;
    int col_mask;
} Keypad;

Keypad keypad_init(int[4], int[4], char[4][4]);
char keypad_key_pressed(Keypad);
void keypad_destroy(Keypad);