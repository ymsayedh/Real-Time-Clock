#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

void Timer_Init();
void Timer_Set(uint8_t h, uint8_t m, uint8_t s);
void Timer_Start();
void Timer_Stop();
void Timer_Reset();
uint8_t Timer_IsFinished();
void Timer_TriggerBuzzer();
void Timer_Update();

#endif /* TIMER_H_ */
