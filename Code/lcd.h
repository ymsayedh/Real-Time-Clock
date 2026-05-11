#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT PORTB
#define LCD_DDR  DDRB
#define RS PD6
#define EN PD7

void LCD_Init();
void LCD_Command(uint8_t cmd);
void LCD_Char(char data);
void LCD_String(char* str);
void LCD_Clear();

#endif
