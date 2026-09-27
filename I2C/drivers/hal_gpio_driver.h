#ifndef HAL_GPIO_DRIVER_H
#define HAL_GPIO_DRIVER_H

#include "stm32f407xx.h"
#include <stdint.h>

/* GPIO mode values */
#define GPIO_PIN_INPUT_MODE         ( (uint32_t)0x00 )
#define GPIO_PIN_OUTPUT_MODE        ( (uint32_t)0x01 )
#define GPIO_PIN_ALT_FUN_MODE       ( (uint32_t)0x02 )

/* GPIO output type */
#define GPIO_PIN_OP_TYPE_PUSHPULL    ( (uint32_t)0x00 )
#define GPIO_PIN_OP_TYPE_OPEN_DRAIN  ( (uint32_t)0x01 )

/* GPIO speed */
#define GPIO_PIN_SPEED_LOW           ( (uint32_t)0x00 )
#define GPIO_PIN_SPEED_MEDIUM        ( (uint32_t)0x01 )
#define GPIO_PIN_SPEED_HIGH          ( (uint32_t)0x02 )
#define GPIO_PIN_SPEED_VERY_HIGH     ( (uint32_t)0x03 )

/* Pull-up / pull-down */
#define GPIO_PIN_NO_PULL_PUSH        ( (uint32_t)0x00 )
#define GPIO_PIN_PULL_UP             ( (uint32_t)0x01 )
#define GPIO_PIN_PULL_DOWN           ( (uint32_t)0x02 )

/* Port defines */
#define GPIO_PORT_A  GPIOA
#define GPIO_PORT_B  GPIOB
#define GPIO_PORT_C  GPIOC
#define GPIO_PORT_D  GPIOD
#define GPIO_PORT_E  GPIOE
#define GPIO_PORT_F  GPIOF
#define GPIO_PORT_G  GPIOG
#define GPIO_PORT_H  GPIOH
#define GPIO_PORT_I  GPIOI

/* Enable macros */
#define _HAL_RCC_GPIOA_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 0))
#define _HAL_RCC_GPIOB_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 1))
#define _HAL_RCC_GPIOC_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 2))
#define _HAL_RCC_GPIOD_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 3))
#define _HAL_RCC_GPIOE_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 4))
#define _HAL_RCC_GPIOF_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 5))
#define _HAL_RCC_GPIOG_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 6))
#define _HAL_RCC_GPIOH_CLK_ENABLE()  (RCC->AHB1ENR |=  (1UL << 7))

/* GPIO pin configuration structure */
typedef struct
{
    uint32_t pin;       /* pin number (0..15) */
    uint32_t mode;      /* GPIO_PIN_*_MODE */
    uint32_t op_type;   /* pushpull/open-drain */
    uint32_t pull;      /* pull-up/pull-down */
    uint32_t speed;     /* speed */
    uint32_t alternate; /* alternate function (AFx) */
} gpio_pin_conf_t;

typedef void (*INT_CALLBCK) (void);

typedef enum
{
    INT_RISING_EDGE,
    INT_FALLING_EDGE,
    INT_RISING_FALLING_EDGE
} int_edge_sel_t;

/* Driver API */
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_conf_t *gpio_pin_conf);
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no);
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t val);
void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint16_t alt_fun_value);
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel, INT_CALLBCK isr);
void hal_gpio_enable_interrupt(uint16_t pin_no);
void hal_gpio_clear_interrupt(uint16_t pin);

#endif
