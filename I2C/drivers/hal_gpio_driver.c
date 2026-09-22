#include "hal_gpio_driver.h"

static INT_CALLBCK callback_ptr = (INT_CALLBCK)0;

static void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx,
                                        uint16_t pin_no,
                                        uint32_t mode)
{
    GPIOx->MODER &= ~(3UL << (2U * pin_no));
    GPIOx->MODER |=  (mode << (2U * pin_no));
}

static void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx,
                                         uint16_t pin_no,
                                         uint32_t speed)
{
    GPIOx->OSPEEDR &= ~(3UL << (2U * pin_no));
    GPIOx->OSPEEDR |=  (speed << (2U * pin_no));
}

static void hal_gpio_configure_pin_otype(GPIO_TypeDef *GPIOx,
                                         uint16_t pin_no,
                                         uint32_t op_type)
{
    GPIOx->OTYPER &= ~(1UL << pin_no);
    GPIOx->OTYPER |=  (op_type << pin_no);
}

static void hal_gpio_configure_pin_pupd(GPIO_TypeDef *GPIOx,
                                        uint16_t pin_no,
                                        uint32_t pupd)
{
    GPIOx->PUPDR &= ~(3UL << (2U * pin_no));
    GPIOx->PUPDR |=  (pupd << (2U * pin_no));
}

void hal_gpio_init(GPIO_TypeDef *GPIOx,
                   gpio_pin_conf_t *gpio_pin_conf)
{
    hal_gpio_configure_pin_mode(GPIOx,
                                (uint16_t)gpio_pin_conf->pin,
                                gpio_pin_conf->mode);

    hal_gpio_configure_pin_speed(GPIOx,
                                 (uint16_t)gpio_pin_conf->pin,
                                 gpio_pin_conf->speed);

    hal_gpio_configure_pin_otype(GPIOx,
                                 (uint16_t)gpio_pin_conf->pin,
                                 gpio_pin_conf->op_type);

    hal_gpio_configure_pin_pupd(GPIOx,
                                (uint16_t)gpio_pin_conf->pin,
                                gpio_pin_conf->pull);
}

void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx,
                           uint16_t pin_no,
                           uint8_t val)
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

uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx,
                               uint16_t pin_no)
{
    return (uint8_t)((GPIOx->IDR >> pin_no) & 1UL);
}

void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx,
                               uint16_t pin_no,
                               uint16_t alt_fun_value)
{
    uint32_t register_index;
    uint32_t bit_position;

    register_index = (uint32_t)pin_no / 8U;
    bit_position = ((uint32_t)pin_no % 8U) * 4U;

    GPIOx->AFR[register_index] &= ~(0x0FUL << bit_position);
    GPIOx->AFR[register_index] |=
        ((uint32_t)alt_fun_value << bit_position);
}

void hal_gpio_configure_interrupt(uint16_t pin_no,
                                  int_edge_sel_t edge_sel,
                                  INT_CALLBCK isr)
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
    else
    {
        EXTI->RTSR |= (1UL << pin_no);
        EXTI->FTSR |= (1UL << pin_no);
    }
}

void hal_gpio_enable_interrupt(uint16_t pin_no)
{
    EXTI->IMR |= (1UL << pin_no);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void hal_gpio_clear_interrupt(uint16_t pin_no)
{
    EXTI->PR |= (1UL << pin_no);
}

void EXTI0_IRQHandler(void)
{
    if (callback_ptr != (INT_CALLBCK)0)
    {
        callback_ptr();
    }
}
