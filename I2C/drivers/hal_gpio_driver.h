#ifndef HAL_GPIO_DRIVER_H
	#define HAL_GPIO_DRIVER_H

	/* MC specific headerfile for stm32f407vgt6 based Discovery board */
	#include "stm32f407xx.h"

	/******************************************************************************/
	/*                                                                            */
	/*                     1. Macros used for GPIO pin Initialization               */
	/*                                                                            */
	/******************************************************************************/
	/* GPIO Mode Settings values*/
	#define GPIO_PIN_INPUT_MODE         ( (uint32_t)0x00 )
	#define GPIO_PIN_OUTPUT_MODE        ( (uint32_t)0x01)
	#define GPIO_PIN_ALT_FUN_MODE       ( (uint32_t)0x02)

	/* GPIO OP type selection values */
	#define GPIO_PIN_OP_TYPE_PUSHPULL    ( (uint32_t)0x00 )
	#define GPIO_PIN_OP_TYPE_OPEN_DRAIN  ( (uint32_t)0x01 )

	/* GPIO Speed type selection values */
	#define GPIO_PIN_SPEED_LOW               ( (uint32_t)0x00 )
	#define GPIO_PIN_SPEED_MEDIUM            ( (uint32_t)0x01 )
	#define GPIO_PIN_SPEED_HIGH              ( (uint32_t)0x02 )
	#define GPIO_PIN_SPEED_VERY_HIGH         ( (uint32_t)0x03 )

	/* GPIO pull up/pull dwn  selection values */
	#define GPIO_PIN_NO_PULL_PUSH   ( (uint32_t)0x00 )
	#define GPIO_PIN_PULL_UP        ( (uint32_t)0x01 )
	#define GPIO_PIN_PULL_DOWN      ( (uint32_t)0x11 )


	/* gpio port address */

	#define GPIO_PORT_A  GPIOA
	#define GPIO_PORT_B  GPIOB
	#define GPIO_PORT_C	 GPIOC
	#define GPIO_PORT_D  GPIOD
	#define GPIO_PORT_E  GPIOE
	#define GPIO_PORT_F  GPIOF
	#define GPIO_PORT_G  GPIOG
	#define GPIO_PORT_H  GPIOH
	#define GPIO_PORT_I  GPIOI

	/* Macros to Enable Clock for diffrent GPIO ports in RCC register */

	#define _HAL_RCC_GPIOA_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 0) )
	#define _HAL_RCC_GPIOB_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 1) )
	#define _HAL_RCC_GPIOC_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 2) )
	#define _HAL_RCC_GPIOD_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 3) )
	#define _HAL_RCC_GPIOE_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 4) )
	#define _HAL_RCC_GPIOF_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 5) )
	#define _HAL_RCC_GPIOG_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 6) )
	#define _HAL_RCC_GPIOH_CLK_ENABLE()       (RCC->AHB1ENR |=  (1 << 7) )
