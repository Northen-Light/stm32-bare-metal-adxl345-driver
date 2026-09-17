#include <stddef.h>
#include "exti.h"
#include "cortex_m3.h"
#include "stm32f103xx.h"

static exti_interrupt_callback_t exti_interrupt_callback = NULL;

void exti_init(void) {
  RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;

  GPIOB_CRL &= ~GPIOB_CRL_PIN0_MASK;
  GPIOB_CRL |= GPIOB_CRL_PIN0_FLOATING_INPUT;

  AFIO_EXTICR1 &= ~AFIO_EXTICR1_EXTI0_MASK;
  AFIO_EXTICR1 |= AFIO_EXTICR1_EXTI0_PB0;

  EXTI_IMR |= EXTI_IMR_LINE0;
  EXTI_RTSR |= EXTI_RTSR_LINE0;
  EXTI_PR = EXTI_PR_LINE0;

  NVIC_ISER0 = NVIC_ISER0_EXTI0;
}

void exti_set_interrupt_callback(exti_interrupt_callback_t callback) {
  exti_interrupt_callback = callback;
}

void EXTI0_IRQHandler(void) {
  EXTI_PR = EXTI_PR_LINE0;
  
  if (exti_interrupt_callback != NULL) {
    exti_interrupt_callback();
  }
}