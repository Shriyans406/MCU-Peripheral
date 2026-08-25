#ifndef __HAL_I2C_DRIVER_H
#define __HAL_I2C_DRIVER_H

#include "stm32f407xx.h"
#include  <stdint.h>


/* I2C device base address */
#define I2C_1 I2C1
#define I2C_2 I2C2
#define I2C_3 I2C3

/* Macros to Enable Clock for diffrent I2C devices */

#define _HAL_RCC_I2C1_CLK_ENABLE()       ( RCC->APB1ENR |=  (1 << 21) )
#define _HAL_RCC_I2C2_CLK_ENABLE()       ( RCC->APB1ENR |= ( 1 << 22) )
#define _HAL_RCC_I2C3_CLK_ENABLE()       ( RCC->APB1ENR |= ( 1 << 23) )


/******************************************************************************/
/*                                                                            */
/*                                I2C                                         */
/*                        Register Bit Defininitions                          */
/******************************************************************************/

/*******************  Bit definition for I2C_CR1 register  ********************/

#define I2C_REG_CR1_POS      ((uint32_t)1 << 11)

#define I2C_REG_CR1_ACK      ((uint32_t)1 << 10)
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE        0

#define I2C_REG_CR1_STOP_GEN               ((uint32_t)1 << 9)
#define I2C_REG_CR1_START_GEN            ((uint32_t)1 << 8)

#define I2C_REG_CR1_NOSTRETCH            ((uint32_t)1 << 7)
#define I2C_ENABLE_CLK_STRETCH   0
#define I2C_DISABLE_CLK_STRETCH  1

#define I2C_REG_CR1_ENABLE_I2C             ((uint32_t)1 << 0)
/*******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_REG_CR2_BUF_INT_ENABLE       ((uint32_t) ( 1 << 10 ) )
#define I2C_REG_CR2_EVT_INT_ENABLE       ((uint32_t) ( 1 << 9 ) )
#define I2C_REG_CR2_ERR_INT_ENABLE       ((uint32_t) ( 1 << 8 ) )

#define I2C_PERIPHERAL_CLK_FREQ_2MHZ      ((uint32_t)2 )
#define I2C_PERIPHERAL_CLK_FREQ_3MHZ      ((uint32_t)3 )
#define I2C_PERIPHERAL_CLK_FREQ_4MHZ     ((uint32_t)4 )
#define I2C_PERIPHERAL_CLK_FREQ_5MHZ     ((uint32_t)5 )
#define I2C_PERIPHERAL_CLK_FREQ_6MHZ     ((uint32_t)6 )
#define I2C_PERIPHERAL_CLK_FREQ_7MHZ     ((uint32_t)7 )
#define I2C_PERIPHERAL_CLK_FREQ_8MHZ     ((uint32_t)8 )
#define I2C_PERIPHERAL_CLK_FREQ_9MHZ     ((uint32_t)9 )
#define I2C_PERIPHERAL_CLK_FREQ_10MHZ     ((uint32_t)10 )
