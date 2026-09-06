/* main.c - adjusted includes to pull in STM32 device/HAL types so the build
   doesn't complain about unknown types (I2C_TypeDef, GPIO_TypeDef, IRQn_Type, ...)
   The correct HAL headers will be selected at compile time based on the
   STM32F0 / STM32F4 macro passed to the compiler.
*/

#include <stdint.h>
#include <string.h>

/* Include the device HAL headers first so device types are available.
   The build uses -DSTM32F0 when targeting F0 series, so pick headers
   conditionally. These headers are expected to be provided by your
   toolchain (CMSIS / STM32 HAL include paths). */
#if defined(STM32F0)
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_gpio.h"
#elif defined(STM32F4)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_gpio.h"
#else
/* Fallback: try F0 headers (your build system should define target macro) */
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_gpio.h"
#endif

/* Project headers (expect these exist in your repo) */
#include "hal_i2c_driver.h"
#include "hal_gpio_driver.h"
#include "i2c_main.h"
#include "led.h"

/* Uncomment to build as I2C master */
//#define I2C_MASTER_MODE_EN

#ifndef SET
#define SET 1
#endif

#define SLAVE_OWN_ADDRESS      (uint8_t)0x53
#define SLAVE_ADDRESS_READ     (uint8_t)0xA7
#define SLAVE_ADDRESS_WRITE    (uint8_t)0xA6

#define GENERAL_CALL_ADDRESS   (uint8_t)0x00

#define MASTER_WRITE_CMD       0xC1
#define MASTER_READ_CMD        0xC2

#define READ_LEN    5
#define WRITE_LEN   5

uint8_t master_tx_buffer[WRITE_LEN] = {0xa5, 0x55, 0xa5, 0x55, 0xb0};
uint8_t master_rx_buffer[READ_LEN];
uint8_t slave_tx_buffer[WRITE_LEN]  = {0xa5, 0x55, 0xa5, 0x55, 0xc0};
uint8_t slave_rx_buffer[READ_LEN];

uint8_t master_write_req;
uint8_t master_read_req;

uint8_t slave_rcv_cmd;

void delay_gen(void)
{
    uint32_t cnt = 500000;
    while (cnt--) { __NOP(); }
}

i2c_handle_t i2c_handle;
int TestReady = 0;

void i2c_gpio_init(void)
{
    gpio_pin_conf_t i2c_scl, i2c_sda;

    _HAL_RCC_GPIOB_CLK_ENABLE();

    i2c_scl.pin = I2C1_SCL_LINE;
    i2c_scl.mode = GPIO_PIN_ALT_FUN_MODE;
    i2c_scl.op_type = GPIO_PIN_OP_TYPE_OPEN_DRAIN;
    i2c_scl.pull = GPIO_PIN_PULL_UP;
    i2c_scl.speed = GPIO_PIN_SPEED_HIGH;

    /* Use HAL / project helper to set AF and init */
    hal_gpio_set_alt_function(GPIOB, I2C1_SCL_LINE, GPIO_PIN_AF4_I2C123);
    hal_gpio_init(GPIOB, &i2c_scl);

    i2c_sda.pin = I2C1_SDA_LINE;
    i2c_sda.mode = GPIO_PIN_ALT_FUN_MODE;
    i2c_sda.op_type = GPIO_PIN_OP_TYPE_OPEN_DRAIN;
    i2c_sda.pull = GPIO_PIN_PULL_UP;
    i2c_sda.speed = GPIO_PIN_SPEED_HIGH;

    hal_gpio_set_alt_function(GPIOB, I2C1_SDA_LINE, GPIO_PIN_AF4_I2C123);
    hal_gpio_init(GPIOB, &i2c_sda);
}

/**
  * @brief  Compares two buffers.
  * @retval 0  : identical; >0 : differs
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

void gpio_btn_interrupt_handler(void)
{
    hal_gpio_clear_interrupt(0);
    TestReady = SET;
}

int main(void)
{
    led_init();

    i2c_gpio_init();

    /* Configure USER Button */
#ifdef I2C_MASTER_MODE_EN
    hal_gpio_configure_interrupt(0, INT_FALLING_EDGE, gpio_btn_interrupt_handler);
#endif

    _HAL_RCC_I2C1_CLK_ENABLE();
    i2c_handle.Instance = I2C_1;
    i2c_handle.Init.ack_enable = I2C_ACK_ENABLE;
    i2c_handle.Init.AddressingMode = I2C_ADDRMODE_7BIT;
    i2c_handle.Init.ClockSpeed = 100000;
    i2c_handle.Init.DutyCycle = I2C_FM_DUTY_2;
    i2c_handle.Init.GeneralCallMode = 0;
    i2c_handle.Init.NoStretchMode = I2C_ENABLE_CLK_STRETCH;
    i2c_handle.Init.OwnAddress1 = SLAVE_OWN_ADDRESS;

    NVIC_EnableIRQ(I2Cx_ER_IRQn);
    NVIC_EnableIRQ(I2Cx_EV_IRQn);

    hal_i2c_init(&i2c_handle);
    hal_i2c_enable_peripheral(i2c_handle.Instance);

    hal_gpio_enable_interrupt(0);

    i2c_handle.State = HAL_I2C_STATE_READY;

#ifdef I2C_MASTER_MODE_EN
    /* Wait for user Button press before starting the communication. Toggles LED3 until then */
    while (TestReady != SET)
    {
        led_toggle(GPIOD, LED_ORANGE);
        delay_gen();
    }
    led_turn_off(GPIOD, LED_ORANGE);
#endif

    while (1)
    {
        while (i2c_handle.State != HAL_I2C_STATE_READY);

#ifdef I2C_MASTER_MODE_EN
        /* Master sequence */
        master_write_req = MASTER_WRITE_CMD;
        hal_i2c_master_tx(&i2c_handle, SLAVE_ADDRESS_WRITE, (uint8_t*)&master_write_req, 1);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        master_write_req = WRITE_LEN;
        hal_i2c_master_tx(&i2c_handle, SLAVE_ADDRESS_WRITE, (uint8_t*)&master_write_req, 1);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        hal_i2c_master_tx(&i2c_handle, SLAVE_ADDRESS_WRITE, master_tx_buffer, WRITE_LEN);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        master_read_req = MASTER_READ_CMD;
        hal_i2c_master_tx(&i2c_handle, SLAVE_ADDRESS_WRITE, (uint8_t*)&master_read_req, 1);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        master_read_req = READ_LEN;
        hal_i2c_master_tx(&i2c_handle, SLAVE_ADDRESS_WRITE, (uint8_t*)&master_read_req, 1);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        memset(master_rx_buffer, 0, READ_LEN);
        hal_i2c_master_rx(&i2c_handle, SLAVE_ADDRESS_READ, master_rx_buffer, READ_LEN);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        if (Buffercmp(slave_tx_buffer, master_rx_buffer, READ_LEN))
        {
            led_turn_on(GPIOD, LED_RED);
        }
        else
        {
            led_toggle(GPIOD, LED_BLUE);
        }
        delay_gen();

#else
        /* Slave sequence */
        hal_i2c_slave_rx(&i2c_handle, &slave_rcv_cmd, 1);
        while (i2c_handle.State != HAL_I2C_STATE_READY);

        if (slave_rcv_cmd == MASTER_WRITE_CMD)
        {
            hal_i2c_slave_rx(&i2c_handle, &slave_rcv_cmd, 1);
            while (i2c_handle.State != HAL_I2C_STATE_READY);

            memset(slave_rx_buffer, 0, sizeof(slave_rx_buffer));
            hal_i2c_slave_rx(&i2c_handle, slave_rx_buffer, slave_rcv_cmd);
            while (i2c_handle.State != HAL_I2C_STATE_READY);

            if (Buffercmp(slave_rx_buffer, master_tx_buffer, READ_LEN))
            {
                led_turn_on(GPIOD, LED_RED);
            }
            else
            {
                led_toggle(GPIOD, LED_BLUE);
            }
        }

        if (slave_rcv_cmd == MASTER_READ_CMD)
        {
            hal_i2c_slave_rx(&i2c_handle, &slave_rcv_cmd, 1);
            while (i2c_handle.State != HAL_I2C_STATE_READY);

            hal_i2c_slave_tx(&i2c_handle, slave_tx_buffer, slave_rcv_cmd);
            while (i2c_handle.State != HAL_I2C_STATE_READY);
        }
#endif
    }

    return 0;
}

/* EXTI callback */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (((uint16_t)0x0001) == GPIO_Pin)
    {
        TestReady = SET;
    }
}

void I2C1_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&i2c_handle);
}

void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&i2c_handle);
}
