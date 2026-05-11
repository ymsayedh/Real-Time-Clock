#include "dio.h"

void DIO_Init() {
    // Configure PORTD buttons as input with internal pull-ups (Active LOW)
    DDRD &= ~((1 << START_STOPWATCH) | (1 << STOP_STOPWATCH) |
              (1 << RESET_STOPWATCH) | (1 << EXIT_MODE) |
              (1 << START_COUNTDOWN));
    PORTD |= (1 << START_STOPWATCH) | (1 << STOP_STOPWATCH) |
             (1 << RESET_STOPWATCH) | (1 << EXIT_MODE) |
             (1 << START_COUNTDOWN);

    // Configure PORTC buttons as input with internal pull-ups (Active LOW)
    DDRC &= ~((1 << SET_TIME) | (1 << SET_DATE) | (1 << SET_ALARM) | (1 << VIEW_ALARMS)| (1 << STOP_ALARM));
    PORTC |= (1 << SET_TIME) | (1 << SET_DATE) | (1 << SET_ALARM) | (1 << VIEW_ALARMS)| (1 << STOP_ALARM);

    // Configure buzzer as output
    DDRB |= (1 << BUZZER_PIN);
    PORTB &= ~(1 << BUZZER_PIN); // Ensure buzzer is off at startup
}

uint8_t Button_Read(char port, uint8_t pin) {
    switch (port) {
        case 'D': return !(PIND & (1 << pin));  // Read PORTD (Active LOW)
        case 'C': return !(PINC & (1 << pin));  // Read PORTC (Active LOW)
        default:  return 0;  // Invalid port
    }
}
