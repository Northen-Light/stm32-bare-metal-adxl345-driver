#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "adxl345.h"
#include "stm32f103xx.h"

void interrupt_init(void);

adxl345_acceleration_t acceleration;
volatile bool data_ready = false;

int main(void) {
  i2c1_init();
  interrupt_init();
  adxl345_init();

  adxl345_stop_measurement();
  adxl345_set_range(ADXL345_RANGE_4G);
  adxl345_set_data_rate(ADXL345_DATA_RATE_200Hz);
  adxl345_set_data_ready_interrupt();
  adxl345_start_measurement();
  
  while (1) {
    if (data_ready) {
      data_ready = false;
      adxl345_read_acceleration(&acceleration);
    }
  }
  return 0;
}

void interrupt_init(void) {
  RCC_APB2ENR |= (1U << 0U);
  RCC_APB2ENR |= (1U << 2U);
  
  AFIO_EXTICR1 &= ~(0xFU << 0U);

  GPIOA_CRL &= ~0xFU;
  GPIOA_CRL |= 0x8U;
  GPIOA_BSRR = (1U << 16U);

  EXTI0_PR = (1U << 0U);
  EXTI0_IMR |= (1U << 0U);
  EXTI0_RTSR |= (1U << 0U);
  NVIC_ISER0 = (1U << 6U);
}