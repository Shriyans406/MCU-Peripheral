#include "hal_gpio_driver.h"

INT_CALLBCK callback_ptr = (INT_CALLBCK)0;

/* ------------------------------ */
/* Internal helper functions       */
/* ------------------------------ */
static void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{
    uint32_t pos = 2U * pin_no;
    GPIOx->MODER &= ~(3UL << pos);
    GPIOx->MODER |= ((mode & 0x3UL) << pos);
}

static void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed)
{
    uint32_t pos = 2U * pin_no;
    GPIOx->OSPEEDR &= ~(3UL << pos);
    GPIOx->OSPEEDR |= ((speed & 0x3UL) << pos);
}

static void hal_gpio_configure_pin_otype(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t op_type)
{
    GPIOx->OTYPER &= ~(1UL << pin_no);
    GPIOx->OTYPER |= ((op_type & 0x1UL) << pin_no);
}

static void hal_gpio_configure_pin_pupd(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t pupd)
{
    uint32_t pos = 2U * pin_no;
    GPIOx->PUPDR &= ~(3UL << pos);
    GPIOx->PUPDR |= ((pupd & 0x3UL) << pos);
}

/* ------------------------------ */
/* Public GPIO APIs                */
/* ------------------------------ */
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_conf_t *gpio_pin_conf)
{
    hal_gpio_configure_pin_mode(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->mode);
    hal_gpio_configure_pin_speed(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->speed);
    hal_gpio_configure_pin_otype(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->op_type);
    hal_gpio_configure_pin_pupd(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->pull);
}

void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t val)
{
    if (val != 0U)
    {
        GPIOx->ODR |= (1UL << pin_no);
    }
    else
    {
        GPIOx->ODR &= ~(1UL << pin_no);
    }
}

uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no)
{
    return (uint8_t)((GPIOx->IDR >> pin_no) & 0x1UL);
}

void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint16_t alt_fun_value)
{
    uint32_t reg_index = (uint32_t)pin_no / 8U;
    uint32_t bit_pos = ((uint32_t)pin_no % 8U) * 4U;

    GPIOx->AFR[reg_index] &= ~(0x0FUL << bit_pos);
    GPIOx->AFR[reg_index] |= ((uint32_t)alt_fun_value << bit_pos);
}

void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel, INT_CALLBCK isr)
{
    callback_ptr = isr;

    if (edge_sel == INT_RISING_EDGE)
    {
        EXTI->RTSR |= (1UL << pin_no);
    }
    else if (edge_sel == INT_FALLING_EDGE)
    {
        EXTI->FTSR |= (1UL << pin_no);
    }
    else if (edge_sel == INT_RISING_FALLING_EDGE)
    {
        EXTI->RTSR |= (1UL << pin_no);
        EXTI->FTSR |= (1UL << pin_no);
    }
    else
    {
        /* TODO: handle invalid edge selection */
    }
}

void hal_gpio_enable_interrupt(uint16_t pin_no)
{
    EXTI->IMR |= (1UL << pin_no);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void hal_gpio_clear_interrupt(uint16_t pin)
{
    EXTI->PR |= (1UL << pin);
}

/* EXTI ISR callback entry */
void EXTI0_IRQHandler(void)
{
    if (callback_ptr)
    {
        callback_ptr();
    }
}
