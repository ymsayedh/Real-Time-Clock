#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

void I2C_Init();
void I2C_Start(uint8_t address);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck();
uint8_t I2C_ReadNack();
void I2C_Stop();

#endif
