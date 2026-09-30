#include "hal_i2c_driver.h"

/* Internal helpers (file scope) */
static void clear_addr_flag(I2C_TypeDef *i2cx);
static uint8_t is_bus_busy(I2C_TypeDef *i2cx);
static uint8_t wait_for_start_flag(I2C_TypeDef *i2cx);
static uint8_t wait_for_address_flag(I2C_TypeDef *i2cx);
static void send_address(I2C_TypeDef *i2cx, uint8_t address);
static void generate_start(I2C_TypeDef *i2cx);
static void generate_stop(I2C_TypeDef *i2cx);
static void configure_buffer_interrupt(I2C_TypeDef *i2cx, uint32_t enable);
static void configure_error_interrupt(I2C_TypeDef *i2cx, uint32_t enable);
static void configure_event_interrupt(I2C_TypeDef *i2cx, uint32_t enable);

/* Basic enable/disable */
void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx)
{
    i2cx->CR1 |= I2C_REG_CR1_ENABLE_I2C;
}

void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx)
{
    i2cx->CR1 &= ~I2C_REG_CR1_ENABLE_I2C;
}

/* Internal helper implementations */
static void send_address(I2C_TypeDef *i2cx, uint8_t address)
{
    i2cx->DR = address;
}

static void generate_start(I2C_TypeDef *i2cx)
{
    i2cx->CR1 |= I2C_REG_CR1_START_GEN;
}

static void generate_stop(I2C_TypeDef *i2cx)
{
    i2cx->CR1 |= I2C_REG_CR1_STOP_GEN;
}

static void configure_buffer_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
    if (enable)
        i2cx->CR2 |= I2C_REG_CR2_BUF_INT_ENABLE;
    else
        i2cx->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
}

static void configure_error_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
    if (enable)
        i2cx->CR2 |= I2C_REG_CR2_ERR_INT_ENABLE;
    else
        i2cx->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
}

static void configure_event_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
    if (enable)
        i2cx->CR2 |= I2C_REG_CR2_EVT_INT_ENABLE;
    else
        i2cx->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
}

static uint8_t is_bus_busy(I2C_TypeDef *i2cx)
{
    return ((i2cx->SR2 & I2C_REG_SR2_BUS_BUSY_FLAG) != 0U) ? 1U : 0U;
}

static uint8_t wait_for_start_flag(I2C_TypeDef *i2cx)
{
    return ((i2cx->SR1 & I2C_REG_SR1_SB_FLAG) != 0U) ? 1U : 0U;
}

static uint8_t wait_for_address_flag(I2C_TypeDef *i2cx)
{
    return ((i2cx->SR1 & I2C_REG_SR1_ADDR_FLAG) != 0U) ? 1U : 0U;
}

static void clear_addr_flag(I2C_TypeDef *i2cx)
{
    volatile uint32_t tmp = i2cx->SR1;
    tmp = i2cx->SR2;
    (void)tmp;
}

/* Public API */
void hal_i2c_init(i2c_handle_t *handle)
{
    /* configure clock registers etc */
    hal_i2c_clk_init(handle->Instance, handle->Init.ClockSpeed, handle->Init.DutyCycle);
    hal_i2c_set_addressing_mode(handle->Instance, handle->Init.AddressingMode);
    hal_i2c_manage_ack(handle->Instance, handle->Init.ack_enable);
    hal_i2c_manage_clock_stretch(handle->Instance, handle->Init.NoStretchMode);
    hal_i2c_set_own_address1(handle->Instance, handle->Init.OwnAddress1);

    handle->State = HAL_I2C_STATE_READY;
}

void hal_i2c_master_tx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
    hal_i2c_enable_peripheral(handle->Instance);

    while (is_bus_busy(handle->Instance)) { /* wait */ }

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;

    handle->State = HAL_I2C_STATE_BUSY_TX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    generate_start(handle->Instance);
    while (!wait_for_start_flag(handle->Instance)) { /* wait */ }

    send_address(handle->Instance, slave_address);
    while (!wait_for_address_flag(handle->Instance)) { /* wait */ }

    clear_addr_flag(handle->Instance);

    configure_buffer_interrupt(handle->Instance, 1U);
    configure_error_interrupt(handle->Instance, 1U);
    configure_event_interrupt(handle->Instance, 1U);
}

void hal_i2c_master_rx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
    hal_i2c_enable_peripheral(handle->Instance);

    while (is_bus_busy(handle->Instance)) { /* wait */ }

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;

    handle->State = HAL_I2C_STATE_BUSY_RX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    handle->Instance->CR1 |= I2C_REG_CR1_ACK;

    generate_start(handle->Instance);
    while (!wait_for_start_flag(handle->Instance)) { /* wait */ }

    send_address(handle->Instance, slave_address);
    while (!wait_for_address_flag(handle->Instance)) { /* wait */ }

    clear_addr_flag(handle->Instance);

    configure_buffer_interrupt(handle->Instance, 1U);
    configure_error_interrupt(handle->Instance, 1U);
    configure_event_interrupt(handle->Instance, 1U);
}

void hal_i2c_slave_tx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
    hal_i2c_enable_peripheral(handle->Instance);

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;

    handle->State = HAL_I2C_STATE_BUSY_TX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    handle->Instance->CR1 |= I2C_REG_CR1_ACK;

    configure_buffer_interrupt(handle->Instance, 1U);
    configure_error_interrupt(handle->Instance, 1U);
    configure_event_interrupt(handle->Instance, 1U);
}

void hal_i2c_slave_rx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
    hal_i2c_enable_peripheral(handle->Instance);

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;

    handle->State = HAL_I2C_STATE_BUSY_RX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    handle->Instance->CR1 |= I2C_REG_CR1_ACK;

    configure_buffer_interrupt(handle->Instance, 1U);
    configure_error_interrupt(handle->Instance, 1U);
    configure_event_interrupt(handle->Instance, 1U);
}

/* IRQ handlers that operate on the provided handle */
void HAL_I2C_EV_IRQHandler(i2c_handle_t *handle)
{
    I2C_TypeDef *i2cx = handle->Instance;

    /* RXNE */
    if (i2cx->SR1 & I2C_REG_SR1_RXNE_FLAG)
    {
        if ((handle->pBuffPtr != 0) && (handle->XferCount > 0U))
        {
            *handle->pBuffPtr = (uint8_t)i2cx->DR;
            handle->pBuffPtr++;
            handle->XferCount--;
        }
    }

    /* TXE */
    if (i2cx->SR1 & I2C_REG_SR1_TXE_FLAG)
    {
        if ((handle->pBuffPtr != 0) && (handle->XferCount > 0U))
        {
            i2cx->DR = *handle->pBuffPtr;
            handle->pBuffPtr++;
            handle->XferCount--;
        }
        else
        {
            generate_stop(i2cx);
            configure_buffer_interrupt(i2cx, 0U);
            configure_event_interrupt(i2cx, 0U);
            handle->State = HAL_I2C_STATE_READY;
        }
    }

    if ((handle->XferCount == 0U) && (handle->State != HAL_I2C_STATE_READY))
    {
        generate_stop(i2cx);
        configure_buffer_interrupt(i2cx, 0U);
        configure_error_interrupt(i2cx, 0U);
        configure_event_interrupt(i2cx, 0U);
        handle->State = HAL_I2C_STATE_READY;
    }
}

void HAL_I2C_ER_IRQHandler(i2c_handle_t *handle)
{
    I2C_TypeDef *i2cx = handle->Instance;

    handle->ErrorCode = i2cx->SR1;

    generate_stop(i2cx);

    configure_buffer_interrupt(i2cx, 0U);
    configure_error_interrupt(i2cx, 0U);
    configure_event_interrupt(i2cx, 0U);

    handle->State = HAL_I2C_STATE_ERROR;
}
