#include "rtc.h"

uint8_t BCD_to_Dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t Dec_to_BCD(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

void RTC_Init() {
    I2C_Init();
}

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    I2C_Start(RTC_ADDRESS);
    I2C_Write(0x00);
    I2C_Write(Dec_to_BCD(seconds));
    I2C_Write(Dec_to_BCD(minutes));
    I2C_Write(Dec_to_BCD(hours));
    I2C_Stop();
}

void RTC_SetDate(uint8_t day, uint8_t month, uint8_t year) {
    I2C_Start(RTC_ADDRESS);
    I2C_Write(0x04);
    I2C_Write(Dec_to_BCD(day));
    I2C_Write(Dec_to_BCD(month));
    I2C_Write(Dec_to_BCD(year));
    I2C_Stop();
}

void RTC_GetTime(uint8_t* hours, uint8_t* minutes, uint8_t* seconds) {
    I2C_Start(RTC_ADDRESS);
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start(RTC_ADDRESS | 1);
    *seconds = BCD_to_Dec(I2C_ReadAck());
    *minutes = BCD_to_Dec(I2C_ReadAck());
    *hours = BCD_to_Dec(I2C_ReadNack());
    I2C_Stop();
}

void RTC_GetDate(uint8_t* day, uint8_t* month, uint8_t* year) {
    I2C_Start(RTC_ADDRESS);
    I2C_Write(0x04);
    I2C_Stop();

    I2C_Start(RTC_ADDRESS | 1);
    *day = BCD_to_Dec(I2C_ReadAck());
    *month = BCD_to_Dec(I2C_ReadAck());
    *year = BCD_to_Dec(I2C_ReadNack());
    I2C_Stop();
}
