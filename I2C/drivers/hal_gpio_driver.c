#include <stdint.h>
#include "hal_gpio_driver.h"

INT_CALLBCK callback_ptr;

void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{
	 GPIOx->MODER   |= (mode << ( 2 * pin_no));
}


void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed)
{

	 GPIOx->OSPEEDR |= (speed << (2 * pin_no));
}


void hal_gpio_configure_pin_otype(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t op_type)
{

	 GPIOx->OTYPER |= (op_type << pin_no);

}
