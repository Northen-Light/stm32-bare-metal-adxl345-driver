#ifndef EXTI_H
#define EXTI_H

typedef void (*exti_interrupt_callback_t)(void);

void exti_init(void);
void exti_set_interrupt_callback(exti_interrupt_callback_t cb);

#endif