#ifndef STM32F103xx_H
#define STM32F103xx_H

#include <stdint.h>

#define RCC_APB2ENR                           *((volatile uint32_t *) 0x40021018U)
#define RCC_APB1ENR                           *((volatile uint32_t *) 0x4002101CU)

#define GPIOB_CRL                             *((volatile uint32_t *) 0x40010C00U) 

#define I2C1_CR1                              *((volatile uint32_t *) 0x40005400U)   
#define I2C1_CR2                              *((volatile uint32_t *) 0x40005404U)   
#define I2C1_DR                               *((volatile uint8_t  *) 0x40005410U)  
#define I2C1_SR1                              *((volatile uint32_t *) 0x40005414U)  
#define I2C1_SR2                              *((volatile uint32_t *) 0x40005418U)
#define I2C_CCR                               *((volatile uint32_t *) 0x4000541CU)  
#define I2C_TRISE                             *((volatile uint32_t *) 0x40005420U)  


#define RCC_APB2ENR_IOPBEN                    (1U << 3)
#define RCC_APB1ENR_I2C1EN                    (1U << 21)

#define GPIOB_CRL_PIN6_MASK                   (0xFU << 24)
#define GPIOB_CRL_PIN6_ALT_OD_2MHZ            (0xEU << 24)
#define GPIOB_CRL_PIN7_MASK                   (0xFU << 28)
#define GPIOB_CRL_PIN7_ALT_OD_2MHZ            (0xEU << 28)

#define I2C1_CR1_RESET                        0x0
#define I2C1_CR2_FREQ_MASK                    (0x3FU << 0)
#define I2C1_CR2_FREQ_PCLK1_8MHZ              (0x8U << 0)
#define I2C_CCR_RESET                         0x0
#define I2C_CCR_SM_100KHZ                     (0x28U << 0)
#define I2C1_TRISE_RESET                      0x2
#define I2C1_TRISE_1000_NS                    (0x9U << 0)  
#define I2C1_CR1_PE                           (1U << 0) 
#define I2C1_CR1_START                        (1U << 8)    
#define I2C1_CR1_STOP                         (1U << 9)   
#define I2C1_CR1_ACK                          (1U << 10) 
#define I2C1_SR1_SB                           (1U << 0)
#define I2C1_SR1_ADDR                         (1U << 1)
#define I2C1_SR1_BTF                          (1U << 2)
#define I2C1_SR1_RXNE                         (1U << 6)
#define I2C1_SR1_TXE                          (1U << 7)

#endif