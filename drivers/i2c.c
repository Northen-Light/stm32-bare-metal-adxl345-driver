#include <stddef.h>
#include "i2c.h"
#include "stm32f103xx.h"

static void i2c1_wait_bus_free(void);
static void i2c1_start(void);
static void i2c1_send_address(uint8_t slave_addr, i2c_dir_t dir);
static void i2c1_clear_addr(void);
static void i2c1_write_byte(uint8_t data);
static void i2c1_wait_for_BTF(void);
static void i2c1_stop(void);
static void i2c1_wait_for_RXNE(void);
static void i2c1_set_ACK(void);
static void i2c1_set_NACK(void);

void i2c1_init(void) {
  RCC_APB2ENR |= (1U << 3U);
  RCC_APB1ENR |= (1U << 21U);

  GPIOB_CRL &= ~(0xFU << 24U);
  GPIOB_CRL |= (0xDU << 24U);
  GPIOB_CRL &= ~(0xFU << 28U);
  GPIOB_CRL |= (0xDU << 28U); 

  I2C1_CR1 &= ~(1U << 0U);
  I2C1_CR2 &= ~0x3FU;
  I2C1_CR2 |= 8U;
  I2C1_CCR = 40U;
  I2C1_TRISE = 9U;
  I2C1_CR1 |= (1U << 0U);
}

uint8_t i2c1_read_register(uint8_t slave_address, uint8_t register_address) {
  i2c1_wait_bus_free();
  i2c1_start();
  i2c1_send_address(slave_address, I2C_WRITE);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);

  i2c1_start();
  i2c1_send_address(slave_address, I2C_READ);
  i2c1_set_NACK();
  i2c1_clear_addr();
  i2c1_stop();
  i2c1_wait_for_RXNE();
  i2c1_set_ACK();
  return I2C1_DR;
}

void i2c1_read_registers(uint8_t slave_address, uint8_t register_address, uint8_t *buffer, uint32_t length) {
  uint32_t remaining;

  if (length < 2U)
    return;

  remaining = length;
  
  i2c1_wait_bus_free();
  i2c1_start();
  i2c1_send_address(slave_address, I2C_WRITE);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);

  i2c1_start();
  i2c1_send_address(slave_address, I2C_READ);
  i2c1_clear_addr();
  i2c1_set_ACK();
  
  while (remaining > 0U) {
    i2c1_wait_for_RXNE();
    *buffer++ = I2C1_DR;
    if (remaining == 2U) {
      i2c1_set_NACK();
      i2c1_stop();
    }
    remaining--;
  }

  i2c1_set_ACK();
}

void i2c1_write_register(uint8_t slave_address, uint8_t register_address, uint8_t value) {
  i2c1_wait_bus_free();
  i2c1_start();
  i2c1_send_address(slave_address, I2C_WRITE);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);
  i2c1_write_byte(value);
  i2c1_wait_for_BTF();
  i2c1_stop();
}

static void i2c1_wait_bus_free(void) {
  while (I2C1_SR2 & (1U << 1U));
}

static void i2c1_start(void) {
  I2C1_CR1 |= (1U << 8U);
  while (!(I2C1_SR1 & (1U << 0U)));
}

static void i2c1_send_address(uint8_t slave_addr, i2c_dir_t dir) {
  I2C1_DR = (uint8_t)(slave_addr << 1U | (uint8_t) dir);
  while (!(I2C1_SR1 & (1U << 1U)));
}

static void i2c1_clear_addr(void) {
  (void) I2C1_SR1;
  (void) I2C1_SR2;
}

static void i2c1_write_byte(uint8_t data) {
  while (!(I2C1_SR1 & (1U << 7U)));
  I2C1_DR = data;
}

static void i2c1_wait_for_BTF(void) {
  while (!(I2C1_SR1 & (1U << 2U)));
}

static void i2c1_stop(void) {
  I2C1_CR1 |= (1U << 9U);
}

static void i2c1_wait_for_RXNE(void) {
  while (!(I2C1_SR1 & (1U << 6U)));
}

static void i2c1_set_ACK(void) {
  I2C1_CR1 |= (1U << 10U);
}

static void i2c1_set_NACK(void) {
  I2C1_CR1 &= ~(1U << 10U);
}