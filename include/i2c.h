#ifndef I2C_H
#define I2C_H

#include "stdint.h"

typedef enum {
  I2C_WRITE = 0,
  I2C_READ
} i2c_dir_t;

void i2c1_init(void);
uint8_t i2c1_read_regsiter(uint8_t slave_address, uint8_t register_address);
void i2c1_write_register(uint8_t slave_address, uint8_t register_address, uint8_t value);
void i2c1_read_registers(uint8_t slave_address, uint8_t register_address, uint8_t *buffer, uint32_t length);

#endif