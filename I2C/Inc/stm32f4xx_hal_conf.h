/*
 * stm32f3xx_hal_conf.h
 *
 *  Created on: Sep 12, 2026
 *      Author: HP
 */

#ifndef STM32F4XX_HAL_CONF_H_
#define STM32F4XX_HAL_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */
#define HAL_MODULE_ENABLED
/* #define HAL_ADC_MODULE_ENABLED      */
/* #define HAL_CAN_MODULE_ENABLED      */
/* #define HAL_CRC_MODULE_ENABLED      */
/* #define HAL_CRYP_MODULE_ENABLED */
/* #define HAL_DAC_MODULE_ENABLED      */
/* #define HAL_DCMI_MODULE_ENABLED     */
#define HAL_DMA_MODULE_ENABLED
/* #define HAL_DMA2D_MODULE_ENABLED */
/* #define HAL_ETH_MODULE_ENABLED      */
#define HAL_FLASH_MODULE_ENABLED
/* #define HAL_NAND_MODULE_ENABLED     */
/* #define HAL_NOR_MODULE_ENABLED      */
/* #define HAL_PCCARD_MODULE_ENABLED   */
/* #define HAL_SRAM_MODULE_ENABLED     */
/* #define HAL_SDRAM_MODULE_ENABLED */
/* #define HAL_HASH_MODULE_ENABLED */
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
/* #define HAL_I2S_MODULE_ENABLED      */
/* #define HAL_IWDG_MODULE_ENABLED     */
/* #define HAL_LTDC_MODULE_ENABLED */
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
/* #define HAL_RNG_MODULE_ENABLED       */
/* #define HAL_RTC_MODULE_ENABLED       */
/* #define HAL_SAI_MODULE_ENABLED */
/* #define HAL_SD_MODULE_ENABLED         */
#define HAL_SPI_MODULE_ENABLED
/* #define HAL_TIM_MODULE_ENABLED       */
/* #define HAL_UART_MODULE_ENABLED      */
/* #define HAL_USART_MODULE_ENABLED     */
/* #define HAL_IRDA_MODULE_ENABLED      */
/* #define HAL_SMARTCARD_MODULE_ENABLED */
/* #define HAL_WWDG_MODULE_ENABLED      */
#define HAL_CORTEX_MODULE_ENABLED
/* #define HAL_PCD_MODULE_ENABLED      */
/* #define HAL_HCD_MODULE_ENABLED      */


 /* ########################## HSE/HSI Values adaptation ##################### */
 /**
   * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
   *        This value is used by the RCC HAL module to compute the system frequency
   *        (when HSE is used as system clock source, directly or through the PLL).
   */
 #if !defined  (HSE_VALUE)
   #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
 #endif /* HSE_VALUE */

 #if !defined  (HSE_STARTUP_TIMEOUT)
   #define HSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /*!< Time out for HSE start up, in ms */
 #endif /* HSE_STARTUP_TIMEOUT */

 /**
   * @brief Internal High Speed oscillator (HSI) value.
   *        This value is used by the RCC HAL module to compute the system frequency
   *        (when HSI is used as system clock source, directly or through the PLL).
   */
 #if !defined  (HSI_VALUE)
   #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
 #endif /* HSI_VALUE */


 /**
   * @brief Internal Low Speed oscillator (LSI) value.
   */
 #if !defined  (LSI_VALUE)
  #define LSI_VALUE  ((uint32_t)32000)
 #endif /* LSI_VALUE */                      /*!< Value of the Internal Low Speed oscillator in Hz
                                              The real value may vary depending on the variations
                                              in voltage and temperature.  */
 /**
   * @brief External Low Speed oscillator (LSE) value.
   */
 #if !defined  (LSE_VALUE)
  #define LSE_VALUE  ((uint32_t)32768)    /*!< Value of the External Low Speed oscillator in Hz */
 #endif /* LSE_VALUE */

 /**
   * @brief External clock source for I2S peripheral
   *        This value is used by the I2S HAL module to compute the I2S clock source
   *        frequency, this source is inserted directly through I2S_CKIN pad.
   */
 #if !defined  (EXTERNAL_CLOCK_VALUE)
   #define EXTERNAL_CLOCK_VALUE    ((uint32_t)12288000) /*!< Value of the Internal oscillator in Hz*/
 #endif /* EXTERNAL_CLOCK_VALUE */

 /* Tip: To avoid modifying this file each time you need to use different HSE,
    ===  you can define the HSE value in your toolchain compiler preprocessor. */


 /* Tip: To avoid modifying this file each time you need to use different HSE,
    ===  you can define the HSE value in your toolchain compiler preprocessor. */

 /* ########################### System Configuration ######################### */
 /**
   * @brief This is the HAL system configuration section
   */
 #define  VDD_VALUE                    ((uint32_t)3300) /*!< Value of VDD in mv */
 #define  TICK_INT_PRIORITY            ((uint32_t)0x0F) /*!< tick interrupt priority */
 #define  USE_RTOS                     0
 #define  PREFETCH_ENABLE              0 /* The prefetch will be enabled in SystemClock_Config(), depending on the used
                                            STM32F405/415/07/417 device: RevA (prefetch must be off) or RevZ (prefetch can be on/off) */
 #define  INSTRUCTION_CACHE_ENABLE     1
 #define  DATA_CACHE_ENABLE            1

#endif /* STM32F4XX_HAL_CONF_H_ */
