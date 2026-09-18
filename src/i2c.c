#include <stdbool.h>
#include "i2c.h"
#include "stm32f103xx.h"
#include "systick.h"

#define I2C_TRANSACTION_TIMEOUT           10

static i2c_status_t i2c1_start(uint32_t start_time_ms, uint32_t timeout_ms);
static i2c_status_t i2c1_send_address(uint8_t address, uint8_t direction, uint32_t start_time_ms, uint32_t timeout_ms);
static void i2c1_request_stop(void);
static i2c_status_t i2c1_write_byte(uint8_t byte, uint32_t start_time_ms, uint32_t timeout_ms);
static i2c_status_t i2c1_read_byte(uint8_t *byte, uint32_t start_time_ms, uint32_t timeout_ms);
static i2c_status_t i2c1_wait_for_flag(uint32_t status_mask, uint32_t start_time_ms, uint32_t timeout_ms);
static i2c_status_t i2c1_wait_for_non_BUSY(uint32_t start_time_ms, uint32_t timeout_ms);
static i2c_status_t i2c1_wait_for_stop(uint32_t start_time_ms, uint32_t timeout_ms);
static void i2c1_set_ACK(void);
static void i2c1_set_NACK(void);
static void i2c1_clear_addr(void);
static i2c_status_t i2c1_detect_error(void);
static i2c_status_t i2c1_abort_transaction(i2c_status_t status);

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
  I2C_CCR = I2C_CCR_RESET;
  I2C_CCR |= I2C_CCR_SM_100KHZ;
  I2C_TRISE = I2C1_TRISE_RESET;
  I2C_TRISE |= I2C1_TRISE_1000_NS;

  I2C1_CR1 |= I2C1_CR1_PE;
}

i2c_status_t i2c1_slave_single_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *byte) {
  i2c_status_t status;
  uint32_t start_time_ms = systick_get_systick_ms();

  status = i2c1_wait_for_non_BUSY(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) return status;

  i2c1_set_ACK();
  
  /* I2C TRANSACTION START */
  status = i2c1_start(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_send_address(slave_address, 0, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_clear_addr();

  status = i2c1_write_byte(register_address, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_wait_for_flag(I2C1_SR1_BTF, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;


    /* REPEATED START */
  status = i2c1_start(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_send_address(slave_address, 1, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_set_NACK();

  uint32_t saved_primask;

  __asm volatile(
    "mrs %0, primask      \n"
    "cpsid i              \n"
    : "=r"(saved_primask)
    :
    : "memory"
  );

  i2c1_clear_addr();
  i2c1_request_stop();

   __asm volatile(
    "msr primask, %0      \n"
    :
    : "r"(saved_primask)
    : "memory"
  );

  status = i2c1_read_byte(byte, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_wait_for_stop(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  /* I2C TRANSACTION END */

  return I2C_STATUS_OK;

  cleanup :
    i2c_status_t cleanup_status = i2c1_abort_transaction(status);
    if (cleanup_status != I2C_STATUS_OK) return cleanup_status;
    return status;
}

i2c_status_t i2c1_slave_multi_byte_read(uint8_t slave_address, uint8_t register_address, uint8_t *bytes, uint8_t length) {
  i2c_status_t status;
  uint8_t byte_index = 0;
  uint32_t start_time_ms = systick_get_systick_ms();

  if (length < 3) {
    return I2C_STATUS_INVALID_ARGUMENT;
  }

  status = i2c1_wait_for_non_BUSY(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) return status;

  i2c1_set_ACK();

  /* I2C TRANSACTION START */
  status = i2c1_start(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_send_address(slave_address, 0, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_clear_addr();

  status = i2c1_write_byte(register_address, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_wait_for_flag(I2C1_SR1_BTF, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

    /* REPEATED START */
  status = i2c1_start(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_send_address(slave_address, 1, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_clear_addr();

  while(byte_index < length - 3) {
    status = i2c1_read_byte(&bytes[byte_index++], start_time_ms, I2C_TRANSACTION_TIMEOUT);
    if (status != I2C_STATUS_OK) goto cleanup;
  }

  status = i2c1_wait_for_flag(I2C1_SR1_BTF, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_set_NACK();

  uint32_t saved_primask;

  __asm volatile(
    "mrs %0, primask      \n"
    "cpsid i              \n"
    : "=r"(saved_primask)
    :
    : "memory"
  );

  bytes[byte_index++] = I2C1_DR;
  i2c1_request_stop();
  bytes[byte_index++] = I2C1_DR;

  __asm volatile(
    "msr primask, %0      \n"
    :
    : "r"(saved_primask)
    : "memory"
  );

  status = i2c1_read_byte(&bytes[byte_index++], start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_wait_for_stop(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  /* I2C TRANSACTION END */
  
  return I2C_STATUS_OK;

  cleanup :
    i2c_status_t cleanup_status = i2c1_abort_transaction(status);
    if (cleanup_status != I2C_STATUS_OK) return cleanup_status;
    return status;
}

i2c_status_t i2c1_slave_single_byte_write(uint8_t slave_address, uint8_t register_address, uint8_t byte) {
  i2c_status_t status;
  uint32_t start_time_ms = systick_get_systick_ms();

  status = i2c1_wait_for_non_BUSY(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) return status;

  /* I2C TRANSACTION START */
  status = i2c1_start(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_send_address(slave_address, 0, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  i2c1_clear_addr();

  status = i2c1_write_byte(register_address, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_write_byte(byte, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  status = i2c1_wait_for_flag(I2C1_SR1_BTF, start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;
  
  i2c1_request_stop();

  status = i2c1_wait_for_stop(start_time_ms, I2C_TRANSACTION_TIMEOUT);
  if (status != I2C_STATUS_OK) goto cleanup;

  /* I2C TRANSACTION END */
                                        
  return I2C_STATUS_OK;

  cleanup :
    i2c_status_t cleanup_status = i2c1_abort_transaction(status);
    if (cleanup_status != I2C_STATUS_OK) return cleanup_status;
    return status;
}

static i2c_status_t i2c1_start(uint32_t start_time_ms, uint32_t timeout_ms) {  
  I2C1_CR1 |= I2C1_CR1_START;
  return i2c1_wait_for_flag(I2C1_SR1_SB, start_time_ms, timeout_ms);
}

static i2c_status_t i2c1_send_address(uint8_t address, uint8_t direction, uint32_t start_time_ms, uint32_t timeout_ms) {
  I2C1_DR = ((address << 1) | direction);
  return i2c1_wait_for_flag(I2C1_SR1_ADDR, start_time_ms, timeout_ms);
}

static void i2c1_request_stop(void) {
  I2C1_CR1 |= I2C1_CR1_STOP;
}

static i2c_status_t i2c1_write_byte(uint8_t byte, uint32_t start_time_ms, uint32_t timeout_ms) {
  i2c_status_t status = i2c1_wait_for_flag(I2C1_SR1_TXE, start_time_ms, timeout_ms);
  if (status != I2C_STATUS_OK) return status;
  I2C1_DR = byte;
  return status;
}

static i2c_status_t i2c1_read_byte(uint8_t *byte, uint32_t start_time_ms, uint32_t timeout_ms) {
  i2c_status_t status = i2c1_wait_for_flag(I2C1_SR1_RXNE, start_time_ms, timeout_ms);
  if (status != I2C_STATUS_OK) return status;
  *byte = I2C1_DR;
  return status;
}

static i2c_status_t i2c1_wait_for_flag(uint32_t status_mask, uint32_t start_time_ms, uint32_t timeout_ms) {
  i2c_status_t status;

  while (1) {
    status = i2c1_detect_error();
    if (status != I2C_STATUS_OK) return status;
    if ((uint32_t)(systick_get_systick_ms() - start_time_ms) >= timeout_ms) return I2C_STATUS_TIMEOUT;
    if ((I2C1_SR1 & status_mask) != 0) return I2C_STATUS_OK;
  }
}

static i2c_status_t i2c1_wait_for_non_BUSY(uint32_t start_time_ms, uint32_t timeout_ms) {
  i2c_status_t status;

  while (1) {
    status = i2c1_detect_error();
    if (status != I2C_STATUS_OK) return status;
    if ((uint32_t)(systick_get_systick_ms() - start_time_ms) >= timeout_ms) return I2C_STATUS_TIMEOUT;
    if ((I2C1_SR2 & I2C1_SR2_BUSY) == 0) return I2C_STATUS_OK;
  }
}

static i2c_status_t i2c1_wait_for_stop(uint32_t start_time_ms, uint32_t timeout_ms) {
  while ((I2C1_CR1 & I2C1_CR1_STOP) != 0) {
    if ((uint32_t)(systick_get_systick_ms() - start_time_ms) >= timeout_ms) {
      return I2C_STATUS_TIMEOUT;
    }
  }
  
  return I2C_STATUS_OK;
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

static i2c_status_t i2c1_detect_error(void) {
  if ((I2C1_SR1 & I2C1_SR1_ARLO) != 0) return I2C_STATUS_ARBITRATION_LOST;
  if ((I2C1_SR1 & I2C1_SR1_BERR) != 0) return I2C_STATUS_BUS_ERROR;
  if ((I2C1_SR1 & I2C1_SR1_AF) != 0) return I2C_STATUS_NACK;
  
  return I2C_STATUS_OK;
}

static i2c_status_t i2c1_abort_transaction(i2c_status_t status) {
  i2c_status_t cleanup_status = I2C_STATUS_OK;
  bool arbitration_lost = (I2C1_SR1 & I2C1_SR1_ARLO) != 0;

  if (!arbitration_lost && 
    (((I2C1_SR1 & (I2C1_SR1_BERR | I2C1_SR1_AF)) != 0)  || (status == I2C_STATUS_TIMEOUT))) {
    i2c1_request_stop();
    cleanup_status = i2c1_wait_for_stop(systick_get_systick_ms(), I2C_TRANSACTION_TIMEOUT);
  }
  
  if ((I2C1_SR1 & I2C1_SR1_ARLO) != 0) I2C1_SR1 &= ~I2C1_SR1_ARLO;
  if ((I2C1_SR1 & I2C1_SR1_BERR) != 0) I2C1_SR1 &= ~I2C1_SR1_BERR;
  if ((I2C1_SR1 & I2C1_SR1_AF) != 0) I2C1_SR1 &= ~I2C1_SR1_AF;
  
  if (cleanup_status != I2C_STATUS_OK) return I2C_STATUS_RECOVERY_FAILED;

  return cleanup_status;
}