#ifndef RTC_H_
#define RTC_H_

#include "i2c.h"

#define RTC_ADDRESS 0xD0

void RTC_Init();
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void RTC_SetDate(uint8_t day, uint8_t month, uint8_t year);
void RTC_GetTime(uint8_t* hours, uint8_t* minutes, uint8_t* seconds);
void RTC_GetDate(uint8_t* day, uint8_t* month, uint8_t* year);

#endif
