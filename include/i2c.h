#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c1_init(void);
void i2c1_single_byte_slave_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte);

#endif