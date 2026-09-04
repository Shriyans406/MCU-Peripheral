/*
 * stm32f4_discovery.h
 *
 *  Created on: Sep 5, 2026
 *      Author: HP
 */

#ifndef STM32F4_DISCOVERY_H_
#define STM32F4_DISCOVERY_H_



#endif /* STM32F4_DISCOVERY_H_ */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_H
#define __STM32F4_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;

typedef enum
{
  BUTTON_KEY = 0,
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */

/**
* @brief  Define for STM32F4_DISCOVERY board
*/
#if !defined (USE_STM32F4_DISCO)
 #define USE_STM32F4_DISCO
#endif

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED4_PIN                         GPIO_PIN_12
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_13
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()

#define LED5_PIN                         GPIO_PIN_14
#define LED5_GPIO_PORT                   GPIOD
#define LED5_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()

#define LED6_PIN                         GPIO_PIN_15
#define LED6_GPIO_PORT                   GPIOD
#define LED6_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED3_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 2) LED5_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 3) LED6_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED5_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED6_GPIO_CLK_DISABLE(); \
                                            }while(0)
/**
  * @}
  */

