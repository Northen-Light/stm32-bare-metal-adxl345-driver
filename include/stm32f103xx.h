#define RCC_APB2ENR       *((volatile uint32_t *) 0x40021018U)
#define RCC_APB1ENR       *((volatile uint32_t *) 0x4002101CU)


#define GPIOB_CRL         *((volatile uint32_t *) 0x40010C00U)
#define GPIOB_BSRR        *((volatile uint32_t *) 0x40010C10U)

#define GPIOA_CRL         *((volatile uint32_t *) 0x40010800U)
#define GPIOA_IDR         *((volatile uint32_t *) 0x40010808U)
#define GPIOA_BSRR        *((volatile uint32_t *) 0x40010810U)

#define I2C1_CR1          *((volatile uint32_t *) 0x40005400U)
#define I2C1_CR2          *((volatile uint32_t *) 0x40005404U)
#define I2C1_DR           *((volatile uint8_t *) 0x40005410U)
#define I2C1_SR1          *((volatile uint32_t *) 0x40005414U)
#define I2C1_SR2          *((volatile uint32_t *) 0x40005418U)
#define I2C1_CCR          *((volatile uint32_t *) 0x4000541CU)
#define I2C1_TRISE        *((volatile uint32_t *) 0x40005420U)

#define AFIO_EXTICR1      *((volatile uint32_t *) 0x40010008U)

#define EXTI0_IMR         *((volatile uint32_t *) 0x40010400U)
#define EXTI0_RTSR        *((volatile uint32_t *) 0x40010408U)
#define EXTI0_PR          *((volatile uint32_t *) 0x40010414U)

#define NVIC_ISER0        *((volatile uint32_t *) 0xE000E100U)  