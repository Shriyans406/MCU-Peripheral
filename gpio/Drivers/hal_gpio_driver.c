#include <stdint.h>
#include "hal_gpio_driver.h"

/**
	* @brief  Configures the mode of a pin : input, output, alt or analog mode
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number
	* @param  mode   : mode to be configured
	* @retval None
	*/
void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{

	GPIOx->MODER   &= ~(0x03 << ( 2 * pin_no));
	GPIOx->MODER   |= (mode << ( 2 * pin_no));
}

/**
	* @brief  Configures the speed of a pin
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number
	* @param  speed   : value of the speed
	* @retval None
	*/
void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed)
{

	 GPIOx->OSPEEDR |= (speed << (2 * pin_no));
}
