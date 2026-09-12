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
  RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;

  GPIOB_CRL &= ~GPIOB_CRL_PIN6_MASK;
  GPIOB_CRL |= GPIOB_CRL_PIN6_ALT_OD_2MHZ;
  GPIOB_CRL &= ~GPIOB_CRL_PIN7_MASK;
  GPIOB_CRL |= GPIOB_CRL_PIN7_ALT_OD_2MHZ;

  I2C1_CR1 = I2C1_CR1_RESET;
  I2C1_CR2 &= ~I2C1_CR2_FREQ_MASK;
  I2C1_CR2 |= I2C1_CR2_FREQ_PCLK1_8MHZ;
  I2C_CCR &= ~I2C_CCR_MASK;
  I2C_CCR |= I2C_CCR_SM_100KHZ;
  I2C_TRISE &= ~I2C1_TRISE_FREQ_MASK;
  I2C_TRISE |= I2C1_TRISE_1000_NS;

  I2C1_CR1 |= I2C1_CR1_PE;
}

void i2c1_slave_single_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte) {
  i2c1_set_ACK();

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

void i2c1_slave_multi_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *bytes, uint8_t length) {
  if (length < 2) {
    return;
  }

  uint8_t idx = 0;

  i2c1_set_ACK();

  i2c1_start();
  i2c1_send_address(slave_address, 0);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);
  i2c1_wait_for_BTF();

  i2c1_start();
  i2c1_send_address(slave_address, 1);
  i2c1_clear_addr();

  while(idx < length - 2) {
    i2c1_read_byte(&bytes[idx++]);
  }

  i2c1_wait_for_BTF();
  i2c1_set_NACK();
  i2c1_read_byte(&bytes[idx++]);
  i2c1_stop();

  i2c1_read_byte(&bytes[idx++]);
  i2c1_read_byte(&bytes[idx++]);
}

void i2c1_slave_single_byte_write(uint8_t slave_addess, uint8_t register_address, uint8_t byte) {
  i2c1_start();
  i2c1_send_address(slave_addess, 0);
  i2c1_clear_addr();
  i2c1_write_byte(register_address);
  i2c1_write_byte(byte);
  i2c1_wait_for_BTF();
  i2c1_stop();
}

static void i2c1_start(void) {
  I2C1_CR1 |= I2C1_CR1_START;
  i2c1_wait_for_SB();
}

static void i2c1_send_address(uint8_t address, uint8_t direction) {
  I2C1_DR = ((address << 1) | direction);
  i2c1_wait_for_ADDR();
}

static void i2c1_stop(void) {
  I2C1_CR1 |= I2C1_CR1_STOP;
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
  while ((I2C1_SR1 & I2C1_SR1_SB) == 0) {}
}

static void i2c1_wait_for_ADDR(void) {
  while ((I2C1_SR1 & I2C1_SR1_ADDR) == 0) {}
}

static void i2c1_wait_for_BTF(void) {
  while ((I2C1_SR1 & I2C1_SR1_BTF) == 0) {}
}

static void i2c1_wait_for_RXNE(void) {
  while ((I2C1_SR1 & I2C1_SR1_RXNE) == 0) {}
}

static void i2c1_wait_for_TXE(void) {
  while ((I2C1_SR1 & I2C1_SR1_TXE) == 0) {}
}

static void i2c1_set_ACK(void) {
  I2C1_CR1 |= I2C1_CR1_ACK;
}

static void i2c1_set_NACK(void) {
  I2C1_CR1 &= ~I2C1_CR1_ACK;
}

static void i2c1_clear_addr(void) {
  (void) I2C1_SR1;
  (void) I2C1_SR2;
}