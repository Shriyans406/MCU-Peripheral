#ifndef __HAL_I2C_DRIVER_H
#define __HAL_I2C_DRIVER_H

#include "stm32f407xx.h"
#include <stdint.h>

#ifndef RESET
#define RESET 0U
#endif

#ifndef SET
#define SET 1U
#endif

#define I2C_1    I2C1
#define I2C_2    I2C2
#define I2C_3    I2C3

#define _HAL_RCC_I2C1_CLK_ENABLE() \
    (RCC->APB1ENR |= (1UL << 21))

#define _HAL_RCC_I2C2_CLK_ENABLE() \
    (RCC->APB1ENR |= (1UL << 22))

#define _HAL_RCC_I2C3_CLK_ENABLE() \
    (RCC->APB1ENR |= (1UL << 23))

#define I2C_REG_CR1_POS              (1UL << 11)
#define I2C_REG_CR1_ACK              (1UL << 10)
#define I2C_ACK_ENABLE               1U
#define I2C_ACK_DISABLE              0U

#define I2C_REG_CR1_STOP_GEN         (1UL << 9)
#define I2C_REG_CR1_START_GEN        (1UL << 8)

#define I2C_REG_CR1_NOSTRETCH        (1UL << 7)
#define I2C_ENABLE_CLK_STRETCH       0U
#define I2C_DISABLE_CLK_STRETCH      1U

#define I2C_REG_CR1_ENABLE_I2C       (1UL << 0)

#define I2C_REG_CR2_BUF_INT_ENABLE   (1UL << 10)
#define I2C_REG_CR2_EVT_INT_ENABLE   (1UL << 9)
#define I2C_REG_CR2_ERR_INT_ENABLE   (1UL << 8)

#define I2C_PERIPHERAL_CLK_FREQ_2MHZ  2U
#define I2C_PERIPHERAL_CLK_FREQ_3MHZ  3U
#define I2C_PERIPHERAL_CLK_FREQ_4MHZ  4U
#define I2C_PERIPHERAL_CLK_FREQ_5MHZ  5U
#define I2C_PERIPHERAL_CLK_FREQ_6MHZ  6U
#define I2C_PERIPHERAL_CLK_FREQ_7MHZ  7U
#define I2C_PERIPHERAL_CLK_FREQ_8MHZ  8U
#define I2C_PERIPHERAL_CLK_FREQ_9MHZ  9U
#define I2C_PERIPHERAL_CLK_FREQ_10MHZ 10U

#define I2C_REG_OAR1_ADDRMODE         (1UL << 15)
#define I2C_ADDRMODE_7BIT             0U
#define I2C_ADDRMODE_10BI             1U

#define I2C_REG_SR1_TIMEOUT_FLAG      (1UL << 14)
#define I2C_REG_SR1_OVR_FLAG          (1UL << 11)
#define I2C_REG_SR1_AF_FAILURE_FLAG   (1UL << 10)
#define I2C_REG_SR1_ARLO_FLAG         (1UL << 9)
#define I2C_REG_SR1_BUS_ERROR_FLAG    (1UL << 8)
#define I2C_REG_SR1_TXE_FLAG          (1UL << 7)
#define I2C_REG_SR1_RXNE_FLAG         (1UL << 6)
#define I2C_REG_SR1_STOP_DETECTION_FLAG (1UL << 4)
#define I2C_REG_SR1_BTF_FLAG          (1UL << 2)
#define I2C_REG_SR1_ADDR_FLAG         (1UL << 1)
#define I2C_REG_SR1_ADDR_SENT_FLAG    (1UL << 1)
#define I2C_REG_SR1_ADDR_MATCHED_FLAG (1UL << 1)
#define I2C_REG_SR1_SB_FLAG           (1UL << 0)

#define I2C_REG_SR2_BUS_BUSY_FLAG     (1UL << 1)
#define I2C_BUS_IS_BUSY               1U
#define I2C_BUS_IS_FREE               0U

#define I2C_REG_SR2_MSL_FLAG          (1UL << 0)
#define I2C_MASTER_MODE               1U
#define I2C_SLAVE_MODE                0U

#define I2C_REG_SR2_TRA_FLAG          (1UL << 2)
#define I2C_RX_MODE                   0U
#define I2C_TX_MODE                   1U

#define I2C_REG_CCR_DUTY              (1UL << 14)
#define I2C_FM_DUTY_16BY9             1U
#define I2C_FM_DUTY_2                 0U

typedef enum
{
    HAL_I2C_STATE_RESET       = 0x00U,
    HAL_I2C_STATE_READY       = 0x01U,
    HAL_I2C_STATE_BUSY        = 0x02U,
    HAL_I2C_STATE_BUSY_TX     = 0x12U,
    HAL_I2C_STATE_BUSY_RX     = 0x22U,
    HAL_I2C_STATE_TIMEOUT     = 0x03U,
    HAL_I2C_STATE_ERROR       = 0x04U
} hal_i2c_state_t;

typedef struct
{
    uint32_t ClockSpeed;
    uint32_t DutyCycle;
    uint32_t OwnAddress1;
    uint32_t AddressingMode;
    uint32_t DualAddressMode;
    uint32_t OwnAddress2;
    uint32_t GeneralCallMode;
    uint32_t NoStretchMode;
    uint32_t ack_enable;
    uint8_t master;
} i2c_init_t;

typedef struct
{
    I2C_TypeDef *Instance;
    i2c_init_t Init;
    uint8_t *pBuffPtr;
    uint32_t XferSize;
    __IO uint32_t XferCount;
    hal_i2c_state_t State;
    uint32_t ErrorCode;
} i2c_handle_t;

typedef enum
{
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

#define UNUSED(x) ((void)(x))

void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx);
void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx);

void hal_i2c_init(i2c_handle_t *handle);
void hal_i2c_manage_ack(I2C_TypeDef *i2cx, uint32_t ack_noack);

void hal_i2c_master_tx(i2c_handle_t *handle,
                       uint8_t slave_address,
                       uint8_t *buffer,
                       uint32_t len);

void hal_i2c_master_rx(i2c_handle_t *handle,
                       uint8_t slave_address,
                       uint8_t *buffer,
                       uint32_t len);

void hal_i2c_slave_tx(i2c_handle_t *handle,
                      uint8_t *buffer,
                      uint32_t len);

void hal_i2c_slave_rx(i2c_handle_t *handle,
                      uint8_t *buffer,
                      uint32_t len);

void HAL_I2C_EV_IRQHandler(i2c_handle_t *handle);
void HAL_I2C_ER_IRQHandler(i2c_handle_t *handle);

#endif
