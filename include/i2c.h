#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c1_init(void);
void i2c1_slave_single_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte);
void i2c1_slave_single_byte_write(uint8_t slave_addess, uint8_t register_address, uint8_t byte);
void i2c1_slave_multi_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *bytes, uint8_t length);

#endif