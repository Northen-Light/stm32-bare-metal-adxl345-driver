#include "systick.h"
#include "systick_internal.h"

static volatile uint32_t systick_ms = 0;

void systick_init(void) {
  SYST_CSR &= ~SYST_CSR_ENABLE;

  SYST_CVR = 0;
  SYST_RVR = SYST_RVR_VAL;
  
  SYST_CSR |= SYST_CSR_TICKINT | SYST_CSR_CLKSOURCE;
  SYST_CSR |= SYST_CSR_ENABLE;
}

uint32_t systick_get_systick_ms(void) {
  return systick_ms;
}

void SysTick_Handler(void) {
  systick_ms++;
}