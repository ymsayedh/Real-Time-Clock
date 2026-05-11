#include "lcd.h"

void LCD_Command(uint8_t cmd) {
    LCD_PORT = (cmd & 0xF0);
    PORTD &= ~(1 << RS);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_us(100);

    LCD_PORT = (cmd << 4);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_ms(2);
}

void LCD_Init() {
	LCD_DDR |= 0xF0;  // Ensure upper 4 bits of PORTB are output
	DDRD |= (1 << RS) | (1 << EN);  // Set PD6, PD7 as output

    _delay_ms(50);
    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
}

void LCD_Char(char data) {
    LCD_PORT = (data & 0xF0);
    PORTD |= (1 << RS);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_us(100);

    LCD_PORT = (data << 4);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_Clear() {
    LCD_Command(0x01);
    _delay_ms(2);
}
