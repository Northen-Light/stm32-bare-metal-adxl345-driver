#include "i2c.h"
#include "stm32f103xx.h"

static void i2c1_start(void);
static void i2c1_send_address(uint8_t address, uint8_t direction);
static void i2c1_stop(void);
static void i2c1_write_byte(uint8_t byte);
static void i2c1_read_byte(uint8_t *byte);
static void i2c1_wait_for_SB(void);
static void i2c1_wait_for_ADDR(void);
static void i2c1_wait_for_BTF(void);
static void i2c1_wait_for_RXNE(void);
static void i2c1_wait_for_TXE(void);
static void i2c1_set_ACK(void);
static void i2c1_set_NACK(void);
static void i2c1_clear_addr(void);

void i2c1_init(void) {
  RCC_APB2ENR |= (1U << 3);
  RCC_APB1ENR |= (1U << 21);

  GPIOB_CRL &= ~(0xFU << 24);
  GPIOB_CRL |= (0xEU << 24);
  GPIOB_CRL &= ~(0xFU << 28);
  GPIOB_CRL |= (0xEU << 28);

  I2C1_CR1 = 0;
  I2C1_CR2 &= ~(0x3FU << 0);
  I2C1_CR2 |= (0x8U << 0);
  I2C_CCR &= ~(0xFFFU << 0);
  I2C_CCR |= (0x28U << 0);
  I2C_TRISE &= ~(0x3FU << 0);
  I2C_TRISE |= (0x9U << 0);

  I2C1_CR1 |= (1U << 0);
}

void i2c1_single_byte_slave_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte) {
  i2c1_start();
  i2c1_send_address(slave_address, 0);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);
  i2c1_wait_for_BTF();

  i2c1_start();
  i2c1_send_address(slave_address, 1);
  i2c1_set_NACK();
  i2c1_clear_addr();
  i2c1_stop();
  i2c1_read_byte(byte);

  i2c1_set_ACK();
}

static void i2c1_start(void) {
  I2C1_CR1 |= (1U << 8);
  i2c1_wait_for_SB();
}

static void i2c1_send_address(uint8_t address, uint8_t direction) {
  I2C1_DR = ((address << 1) | direction);
  i2c1_wait_for_ADDR();
}

static void i2c1_stop(void) {
  I2C1_CR1 |= (1U << 9);
}

static void i2c1_write_byte(uint8_t byte) {
  i2c1_wait_for_TXE();
  I2C1_DR = byte;
}

static void i2c1_read_byte(uint8_t *byte) {
  i2c1_wait_for_RXNE();
  *byte = I2C1_DR;
}

static void i2c1_wait_for_SB(void) {
  while ((I2C1_SR1 & (1U << 0)) == 0) {}
}

static void i2c1_wait_for_ADDR(void) {
  while ((I2C1_SR1 & (1U << 1)) == 0) {}
}

static void i2c1_wait_for_BTF(void) {
  while ((I2C1_SR1 & (1U << 2)) == 0) {}
}

static void i2c1_wait_for_RXNE(void) {
  while ((I2C1_SR1 & (1U << 6)) == 0) {}
}

static void i2c1_wait_for_TXE(void) {
  while ((I2C1_SR1 & (1U << 7)) == 0) {}
}

static void i2c1_set_ACK(void) {
  I2C1_CR1 |= (1U << 10);
}

static void i2c1_set_NACK(void) {
  I2C1_CR1 &= ~(1U << 10);
}

static void i2c1_clear_addr(void) {
  (void) I2C1_SR1;
  (void) I2C1_SR2;
}