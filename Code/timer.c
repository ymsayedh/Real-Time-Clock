#include "timer.h"
#include "dio.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // Required for ISR

static uint8_t countdown_hours = 0;
static uint8_t countdown_minutes = 0;
static uint8_t countdown_seconds = 0;
static uint8_t timer_running = 0;

void Timer_Init() {
    cli(); // Disable global interrupts during setup
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, Prescaler 1024
    OCR1A = 15625;  // 1-second interval at 16MHz clock
    TIMSK |= (1 << OCIE1A);  // Enable Timer1 Compare Match Interrupt
    sei(); // Re-enable global interrupts
}

void Timer_Set(uint8_t h, uint8_t m, uint8_t s) {
    countdown_hours = h;
    countdown_minutes = m;
    countdown_seconds = s;
}

void Timer_Start() {
    timer_running = 1;
}

void Timer_Stop() {
    timer_running = 0;
}

void Timer_Reset() {
    timer_running = 0;
    countdown_hours = 0;
    countdown_minutes = 0;
    countdown_seconds = 0;
}

uint8_t Timer_IsFinished() {
    return (countdown_hours == 0 && countdown_minutes == 0 && countdown_seconds == 0);
}

void Timer_TriggerBuzzer() {
    PORTB |= (1 << BUZZER_PIN); // Turn on buzzer
    _delay_ms(5000); // Buzzer on for 5 seconds
    PORTB &= ~(1 << BUZZER_PIN); // Turn off buzzer
}

void Timer_Update() {
    if (Timer_IsFinished()) {
        timer_running = 0;
        Timer_TriggerBuzzer();
    }

    LCD_Clear();
    LCD_String("Time Left: ");
    LCD_Char((countdown_hours / 10) + '0');
    LCD_Char((countdown_hours % 10) + '0');
    LCD_Char(':');
    LCD_Char((countdown_minutes / 10) + '0');
    LCD_Char((countdown_minutes % 10) + '0');
    LCD_Char(':');
    LCD_Char((countdown_seconds / 10) + '0');
    LCD_Char((countdown_seconds % 10) + '0');
}

// ✅ Fix: ISR to correctly decrement time every 1 second
ISR(TIMER1_COMPA_vect) {
    if (timer_running && !Timer_IsFinished()) {
        if (countdown_seconds > 0) {
            countdown_seconds--;
        } else {
            if (countdown_minutes > 0) {
                countdown_minutes--;
                countdown_seconds = 59;
            } else if (countdown_hours > 0) {
                countdown_hours--;
                countdown_minutes = 59;
                countdown_seconds = 59;
            }
        }
    }
}
