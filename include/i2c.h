#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  I2C_STATUS_OK,
  I2C_STATUS_BUS_ERROR,
  I2C_STATUS_ARBITRATION_LOST,
  I2C_STATUS_NACK,
  I2C_STATUS_INVALID_ARGUMENT,
  I2C_STATUS_TIMEOUT,
  I2C_STATUS_RECOVERY_FAILED,
} i2c_status_t;

void i2c1_init(void);
i2c_status_t i2c1_slave_register_single_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte);
i2c_status_t i2c1_slave_register_multi_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *bytes, uint8_t length);
i2c_status_t i2c1_slave_register_single_byte_write(uint8_t slave_addess, uint8_t register_address, uint8_t byte);

#endif