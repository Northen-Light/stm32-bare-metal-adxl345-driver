#include <stdbool.h>
#include <stdint.h>
#include "stm32f103xx.h"
extern int main(void);

extern char _estack;
extern char _sidata;
extern char _sdata;
extern char _edata;
extern char _sbss;
extern char _ebss;

extern volatile bool data_ready;

void Reset_Handler(void);
void HardFault_Handler(void);
void EXTI0_IRQHandler(void);

__attribute__((used, section(".isr_vector")))
uintptr_t vector_table[] = {
  (uintptr_t) &_estack,
  (uintptr_t) Reset_Handler,
  0U,
  (uintptr_t) HardFault_Handler,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  0U,
  (uintptr_t) EXTI0_IRQHandler
};

void Reset_Handler(void) {
  char *start_data_src =  &_sidata;
  char *start_data_dest = &_sdata;
  char *end_data_dest = &_edata;

  while (start_data_dest != end_data_dest) {
    *start_data_dest++ = *start_data_src++;
  }

  char *start_bss_dest = &_sbss;
  char *end_bss_dest = &_ebss;

  while (start_bss_dest != end_bss_dest) {
    *start_bss_dest++ = 0;
  }

  main();

  while(1);
}

void HardFault_Handler(void) {
  while(1);
}

void EXTI0_IRQHandler(void) {
  EXTI0_PR = (1U << 0U);
  data_ready = true;
}
