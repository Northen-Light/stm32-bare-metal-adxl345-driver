#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C1_CR1_PE           (1U << 0U)
#define I2C1_CR2_FREQ_MASK    0x3FU
#define I2C1_CR2_FREQ_8MHZ    0x8U  

#define I2C1_TRISE_SM_100KHZ  9U

#define I2C1_CCR_SM_100KHZ    40U

#define I2C1_CR1_START        (1U << 8U)
#define I2C1_CR1_STOP         (1U << 9U)
#define I2C1_CR1_ACK          (1U << 10U)

#define I2C1_SR1_SB           (1U << 0U)
#define I2C1_SR1_ADDR         (1U << 1U)     
#define I2C1_SR1_BTF          (1U << 2U)
#define I2C1_SR1_RXNE         (1U << 6U)
#define I2C1_SR1_TXE          (1U << 7U)

#define I2C1_SR2_BUSY         (1U << 1U)     

typedef enum {
  I2C_WRITE = 0,
  I2C_READ
} i2c_dir_t;

void i2c1_init(void);
uint8_t i2c1_read_register(uint8_t slave_address, uint8_t register_address);
void i2c1_write_register(uint8_t slave_address, uint8_t register_address, uint8_t value);
void i2c1_read_registers(uint8_t slave_address, uint8_t register_address, uint8_t *buffer, uint32_t length);

#endif