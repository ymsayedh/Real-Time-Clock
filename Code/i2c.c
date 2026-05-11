#include "i2c.h"

void I2C_Init() {
    TWBR = 32;  // Set clock speed (100kHz for 8MHz CPU)
    TWSR = 0x00;
}

void I2C_Start(uint8_t address) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t I2C_ReadAck() {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_ReadNack() {
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void I2C_Stop() {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
}
