#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>
#include <util/delay.h>

// Define keypad port
#define KEYPAD_PORT PORTA
#define KEYPAD_DDR  DDRA
#define KEYPAD_PIN  PINA

// Function prototypes
void Keypad_Init();
char Keypad_GetKey();

#endif /* KEYPAD_H_ */
