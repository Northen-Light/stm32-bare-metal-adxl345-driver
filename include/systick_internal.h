#ifndef SYSTICK_INTERNAL_H
#define SYSTICK_INTERNAL_H

#define SYST_CSR                                *((volatile uint32_t *)0xE000E010U)
#define SYST_RVR                                *((volatile uint32_t *)0xE000E014U)
#define SYST_CVR                                *((volatile uint32_t *)0xE000E018U)

#define SYST_CSR_ENABLE                         (1U << 0)
#define SYST_CSR_TICKINT                        (1U << 1)
#define SYST_CSR_CLKSOURCE                      (1U << 2)
#define SYSTEM_CORE_CLOCK_HZ                    8000000
#define SYSTICK_HZ                              1000
#define SYST_RVR_VAL                            ((SYSTEM_CORE_CLOCK_HZ / SYSTICK_HZ) - 1)                           

void systick_init(void);
void SysTick_Handler(void);

#endif