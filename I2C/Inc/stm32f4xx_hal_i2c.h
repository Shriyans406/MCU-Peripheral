/*
 * stm32f4xx_hal_i2c.h
 *
 *  Created on: Sep 15, 2026
 *      Author: HP
 */

#ifndef STM32F4XX_HAL_I2C_H_
#define STM32F4XX_HAL_I2C_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup I2C_Exported_Types I2C Exported Types
  * @{
  */


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup I2C_Exported_Constants I2C Exported Constants
  * @{
  */

/** @defgroup I2C_Error_Code I2C Error Code
  * @brief    I2C Error Code
  * @{
  */

 /** @defgroup I2C_Error_Code I2C Error Code
   * @brief    I2C Error Code
   * @{
   */
 #define HAL_I2C_ERROR_NONE       ((uint32_t)0x00000000)    /*!< No error           */
 #define HAL_I2C_ERROR_BERR       ((uint32_t)0x00000001)    /*!< BERR error         */
 #define HAL_I2C_ERROR_ARLO       ((uint32_t)0x00000002)    /*!< ARLO error         */
 #define HAL_I2C_ERROR_AF         ((uint32_t)0x00000004)    /*!< AF error           */
 #define HAL_I2C_ERROR_OVR        ((uint32_t)0x00000008)    /*!< OVR error          */
 #define HAL_I2C_ERROR_DMA        ((uint32_t)0x00000010)    /*!< DMA transfer error */
 #define HAL_I2C_ERROR_TIMEOUT    ((uint32_t)0x00000020)    /*!< Timeout Error      */
 /**
   * @}
   */

 /** @defgroup I2C_duty_cycle_in_fast_mode I2C duty cycle in fast mode
   * @{
   */
 #define I2C_DUTYCYCLE_2                 ((uint32_t)0x00000000)
 #define I2C_DUTYCYCLE_16_9              I2C_CCR_DUTY
 /**
   * @}
   */

 /** @defgroup I2C_addressing_mode I2C addressing mode
   * @{
   */
 #define I2C_ADDRESSINGMODE_7BIT         ((uint32_t)0x00004000)
 #define I2C_ADDRESSINGMODE_10BIT        (I2C_OAR1_ADDMODE | ((uint32_t)0x00004000))
 /**
   * @}
   */

 /** @defgroup I2C_dual_addressing_mode  I2C dual addressing mode
   * @{
   */
 #define I2C_DUALADDRESS_DISABLE        ((uint32_t)0x00000000)
 #define I2C_DUALADDRESS_ENABLE         I2C_OAR2_ENDUAL
 /**
   * @}
   */

 /** @defgroup I2C_general_call_addressing_mode I2C general call addressing mode
   * @{
   */
 #define I2C_GENERALCALL_DISABLE        ((uint32_t)0x00000000)
 #define I2C_GENERALCALL_ENABLE         I2C_CR1_ENGC
 /**
   * @}
   */
 ** @defgroup I2C_nostretch_mode I2C nostretch mode
   * @{
   */
 #define I2C_NOSTRETCH_DISABLE          ((uint32_t)0x00000000)
 #define I2C_NOSTRETCH_ENABLE           I2C_CR1_NOSTRETCH
 /**
   * @}
   */

 /** @defgroup I2C_Memory_Address_Size I2C Memory Address Size
   * @{
   */
 #define I2C_MEMADD_SIZE_8BIT            ((uint32_t)0x00000001)
 #define I2C_MEMADD_SIZE_16BIT           ((uint32_t)0x00000010)
 /**
   * @}
   */

 /** @defgroup I2C_Interrupt_configuration_definition I2C Interrupt configuration definition
   * @{
   */
 #define I2C_IT_BUF                      I2C_CR2_ITBUFEN
 #define I2C_IT_EVT                      I2C_CR2_ITEVTEN
 #define I2C_IT_ERR                      I2C_CR2_ITERREN
 /**
   * @}
   */



#endif /* STM32F4XX_HAL_I2C_H_ */
