#include "hal_i2c_driver.h"

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

void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx)
{
    i2cx->CR1 |= I2C_REG_CR1_ENABLE_I2C;
}

void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx)
{
    i2cx->CR1 &= ~I2C_REG_CR1_ENABLE_I2C;
}

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
    volatile uint32_t tmp1 = i2cx->SR1;
    volatile uint32_t tmp2 = i2cx->SR2;
    (void)tmp1;
    (void)tmp2;
}

void hal_i2c_manage_ack(I2C_TypeDef *i2cx, uint32_t ack_noack)
{
    if (ack_noack == I2C_ACK_ENABLE)
        i2cx->CR1 |= I2C_REG_CR1_ACK;
    else
        i2cx->CR1 &= ~I2C_REG_CR1_ACK;
}

void hal_i2c_clk_init(I2C_TypeDef *i2cx, uint32_t clk_speed, uint32_t duty_cycle)
{
    uint32_t pclk1 = HAL_I2C_PCLK1_MHZ * 1000000UL;
    uint32_t ccr;
    uint32_t trise;

    /* The peripheral must be disabled while CR2/CCR/TRISE are programmed. */
    hal_i2c_disable_peripheral(i2cx);

    i2cx->CR2 &= ~0x3FUL;
    i2cx->CR2 |= HAL_I2C_PCLK1_MHZ;

    if (clk_speed <= 100000UL)
    {
        /* Standard mode: Thigh = Tlow = CCR * TPCLK1 */
        ccr = pclk1 / (2UL * clk_speed);
        if (ccr < 4UL)
            ccr = 4UL;
        i2cx->CCR = ccr & 0xFFFUL;

        trise = HAL_I2C_PCLK1_MHZ + 1UL;
    }
    else
    {
        /* Fast mode */
        if (duty_cycle == I2C_FM_DUTY_2)
        {
            ccr = pclk1 / (3UL * clk_speed);
            i2cx->CCR = I2C_REG_CCR_FS | (ccr & 0xFFFUL);
        }
        else
        {
            ccr = pclk1 / (25UL * clk_speed);
            i2cx->CCR = I2C_REG_CCR_FS | I2C_REG_CCR_DUTY | (ccr & 0xFFFUL);
        }

        trise = (HAL_I2C_PCLK1_MHZ * 300UL / 1000UL) + 1UL;
    }

    i2cx->TRISE = trise & 0x3FUL;
}

void hal_i2c_set_addressing_mode(I2C_TypeDef *i2cx, uint32_t addr_mode)
{
    if (addr_mode == I2C_ADDRMODE_7BIT)
        i2cx->OAR1 &= ~I2C_REG_OAR1_ADDRMODE;
    else
        i2cx->OAR1 |= I2C_REG_OAR1_ADDRMODE;
}

void hal_i2c_manage_clock_stretch(I2C_TypeDef *i2cx, uint32_t no_stretch)
{
    if (no_stretch == I2C_DISABLE_CLK_STRETCH)
        i2cx->CR1 |= I2C_REG_CR1_NOSTRETCH;
    else
        i2cx->CR1 &= ~I2C_REG_CR1_NOSTRETCH;
}

void hal_i2c_set_own_address1(I2C_TypeDef *i2cx, uint32_t own_address)
{
    /* 7-bit address occupies OAR1[7:1]; bit 14 must be kept set by software. */
    i2cx->OAR1 &= ~(0xFFUL << 0);
    i2cx->OAR1 |= ((own_address & 0x7FUL) << 1) | (1UL << 14);
}

void hal_i2c_init(i2c_handle_t *handle)
{
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

    while (is_bus_busy(handle->Instance))
    {
    }

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;

    handle->State = HAL_I2C_STATE_BUSY_TX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    generate_start(handle->Instance);

    while (!wait_for_start_flag(handle->Instance))
    {
    }

    send_address(handle->Instance, slave_address);

    while (!wait_for_address_flag(handle->Instance))
    {
    }

    clear_addr_flag(handle->Instance);

    configure_buffer_interrupt(handle->Instance, 1U);
    configure_error_interrupt(handle->Instance, 1U);
    configure_event_interrupt(handle->Instance, 1U);
}

void hal_i2c_master_rx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
    hal_i2c_enable_peripheral(handle->Instance);

    while (is_bus_busy(handle->Instance))
    {
    }

    handle->Instance->CR1 &= ~I2C_REG_CR1_POS;
    handle->Instance->CR1 |= I2C_REG_CR1_ACK;

    handle->State = HAL_I2C_STATE_BUSY_RX;
    handle->pBuffPtr = buffer;
    handle->XferCount = len;
    handle->XferSize = len;

    generate_start(handle->Instance);

    while (!wait_for_start_flag(handle->Instance))
    {
    }

    send_address(handle->Instance, slave_address);

    while (!wait_for_address_flag(handle->Instance))
    {
    }

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

void HAL_I2C_EV_IRQHandler(i2c_handle_t *handle)
{
    I2C_TypeDef *i2cx = handle->Instance;

    if (i2cx->SR1 & I2C_REG_SR1_RXNE_FLAG)
    {
        if ((handle->pBuffPtr != (uint8_t *)0) && (handle->XferCount > 0U))
        {
            *handle->pBuffPtr = (uint8_t)i2cx->DR;
            handle->pBuffPtr++;
            handle->XferCount--;
        }
    }

    if (i2cx->SR1 & I2C_REG_SR1_TXE_FLAG)
    {
        if ((handle->pBuffPtr != (uint8_t *)0) && (handle->XferCount > 0U))
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
