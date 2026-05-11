#include "keypad.h"

const char KEYPAD_MAP[4][4] = {
    {'7', '8', '9', 'A'},
    {'4', '5', '6', 'B'},
    {'1', '2', '3', 'C'},
    {'*', '0', '=', 'D'}
};

void Keypad_Init() {
    KEYPAD_DDR = 0xF0;  // Upper 4 bits as output, lower 4 as input
    KEYPAD_PORT = 0xFF; // Enable pull-up resistors
}

char Keypad_GetKey() {
    for (uint8_t row = 0; row < 4; row++) {
        KEYPAD_PORT = ~(1 << (row + 4)); // Set one row low at a time
        _delay_ms(5);

        for (uint8_t col = 0; col < 4; col++) {
            if (!(KEYPAD_PIN & (1 << col))) { // If key is pressed
                while (!(KEYPAD_PIN & (1 << col))); // Wait for key release
                return KEYPAD_MAP[row][col];
            }
        }
    }
    return 0; // No key pressed
}
