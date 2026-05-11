#ifndef DIO_H_
#define DIO_H_

#include <avr/io.h>

// Button Definitions (Active LOW)
#define START_STOPWATCH   PD0
#define STOP_STOPWATCH    PD1
#define RESET_STOPWATCH   PD2
#define EXIT_MODE         PD3
#define START_COUNTDOWN   PD4

#define SET_DATE          PC2
#define SET_TIME          PC3
#define SET_ALARM         PC4
#define VIEW_ALARMS       PC5
#define STOP_ALARM		  PC6
// Buzzer Pin
#define BUZZER_PIN        PB1

void DIO_Init();
uint8_t Button_Read(char port, uint8_t pin);

#endif /* DIO_H_ */
