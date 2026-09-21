#include <stdint.h>
#include <string.h>

#include "stm32f407xx.h"
#include "hal_i2c_driver.h"
#include "hal_gpio_driver.h"

/*
 * Uncomment this line when building the master firmware.
 * Leave it commented when building the slave firmware.
 */
/* #define I2C_MASTER_MODE_EN */

/* -------------------------------------------------------------------------- */
/* STM32F407 I2C1 configuration                                               */
/* -------------------------------------------------------------------------- */

/*
 * STM32F407 I2C1:
 *
 * SCL: PB6
 * SDA: PB9
 * Alternate function: AF4
 */
#define I2C1_SCL_LINE       (6U)
#define I2C1_SDA_LINE       (9U)
#define I2C1_GPIO_AF        (4U)

/* -------------------------------------------------------------------------- */
/* Application configuration                                                  */
/* -------------------------------------------------------------------------- */

#define SLAVE_OWN_ADDRESS   ((uint8_t)0x53U)

#define SLAVE_ADDRESS_READ  ((uint8_t)0xA7U)
#define SLAVE_ADDRESS_WRITE ((uint8_t)0xA6U)

#define MASTER_WRITE_CMD    ((uint8_t)0xC1U)
#define MASTER_READ_CMD     ((uint8_t)0xC2U)

#define READ_LEN            (5U)
#define WRITE_LEN           (5U)

/* -------------------------------------------------------------------------- */
/* Driver functions not declared in the supplied driver header                */
/* -------------------------------------------------------------------------- */

extern void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx);

/* -------------------------------------------------------------------------- */
/* Global variables                                                           */
/* -------------------------------------------------------------------------- */

static i2c_handle_t i2c_handle;

static uint8_t master_tx_buffer[WRITE_LEN] =
{
    0xA5U,
    0x55U,
    0xA5U,
    0x55U,
    0xB0U
};

static uint8_t master_rx_buffer[READ_LEN];

static uint8_t slave_tx_buffer[WRITE_LEN] =
{
    0xA5U,
    0x55U,
    0xA5U,
    0x55U,
    0xC0U
};

static uint8_t slave_rx_buffer[READ_LEN];

static uint8_t master_write_request;
static uint8_t master_read_request;
static uint8_t slave_received_command;

static volatile uint8_t test_ready = 0U;

/* -------------------------------------------------------------------------- */
/* Local functions                                                            */
/* -------------------------------------------------------------------------- */

static void delay_gen(void)
{
    volatile uint32_t count = 500000U;

    while (count > 0U)
    {
        count--;
    }
}

static uint8_t buffer_compare(const uint8_t *buffer1,
                              const uint8_t *buffer2,
                              uint16_t length)
{
    uint16_t index;

    for (index = 0U; index < length; index++)
    {
        if (buffer1[index] != buffer2[index])
        {
            return 1U;
        }
    }

    return 0U;
}

static void i2c_gpio_init(void)
{
    gpio_pin_conf_t i2c_scl;
    gpio_pin_conf_t i2c_sda;

    memset(&i2c_scl, 0, sizeof(i2c_scl));
    memset(&i2c_sda, 0, sizeof(i2c_sda));

    /* Enable GPIOB clock. */
    _HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure PB6 as I2C1_SCL. */
    i2c_scl.pin       = I2C1_SCL_LINE;
    i2c_scl.mode      = GPIO_PIN_ALT_FUN_MODE;
    i2c_scl.op_type   = GPIO_PIN_OP_TYPE_OPEN_DRAIN;
    i2c_scl.pull      = GPIO_PIN_PULL_UP;
    i2c_scl.speed     = GPIO_PIN_SPEED_HIGH;
    i2c_scl.alternate = I2C1_GPIO_AF;

    hal_gpio_set_alt_function(GPIOB,
                              I2C1_SCL_LINE,
                              I2C1_GPIO_AF);

    hal_gpio_init(GPIOB, &i2c_scl);

    /* Configure PB9 as I2C1_SDA. */
    i2c_sda.pin       = I2C1_SDA_LINE;
    i2c_sda.mode      = GPIO_PIN_ALT_FUN_MODE;
    i2c_sda.op_type   = GPIO_PIN_OP_TYPE_OPEN_DRAIN;
    i2c_sda.pull      = GPIO_PIN_PULL_UP;
    i2c_sda.speed     = GPIO_PIN_SPEED_HIGH;
    i2c_sda.alternate = I2C1_GPIO_AF;

    hal_gpio_set_alt_function(GPIOB,
                              I2C1_SDA_LINE,
                              I2C1_GPIO_AF);

    hal_gpio_init(GPIOB, &i2c_sda);
}

static void gpio_button_interrupt_handler(void)
{
    hal_gpio_clear_interrupt(0U);
    test_ready = SET;
}

/* -------------------------------------------------------------------------- */
/* Main application                                                           */
/* -------------------------------------------------------------------------- */

int main(void)
{
    i2c_gpio_init();

#ifdef I2C_MASTER_MODE_EN
    /*
     * Configure the user button only in master mode.
     * GPIO pin 0 is used because that is what the original application used.
     */
    hal_gpio_configure_interrupt(0U,
                                 INT_FALLING_EDGE,
                                 gpio_button_interrupt_handler);
#endif

    /* Enable the I2C1 peripheral clock. */
    _HAL_RCC_I2C1_CLK_ENABLE();

    /* Select I2C1. */
    i2c_handle.Instance = I2C1;

    /* Configure I2C1. */
    i2c_handle.Init.ClockSpeed      = 100000U;
    i2c_handle.Init.DutyCycle       = I2C_FM_DUTY_2;
    i2c_handle.Init.OwnAddress1     = SLAVE_OWN_ADDRESS;
    i2c_handle.Init.AddressingMode  = I2C_ADDRMODE_7BIT;
    i2c_handle.Init.GeneralCallMode = 0U;
    i2c_handle.Init.NoStretchMode   = I2C_ENABLE_CLK_STRETCH;
    i2c_handle.Init.ack_enable      = I2C_ACK_ENABLE;

    /*
     * STM32F407 uses separate I2C event and error interrupts.
     */
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_EnableIRQ(I2C1_ER_IRQn);

#ifdef I2C_MASTER_MODE_EN
    hal_gpio_enable_interrupt(0U);
#endif

    /* Initialize and enable the I2C peripheral. */
    hal_i2c_init(&i2c_handle);
    hal_i2c_enable_peripheral(i2c_handle.Instance);

    i2c_handle.State = HAL_I2C_STATE_READY;

#ifdef I2C_MASTER_MODE_EN

    /*
     * Wait for the button before beginning master communication.
     */
    while (test_ready != SET)
    {
        delay_gen();
    }

#endif

    while (1)
    {
        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
            /* Wait until the previous transaction completes. */
        }

#ifdef I2C_MASTER_MODE_EN

        /* Send the write command. */
        master_write_request = MASTER_WRITE_CMD;

        hal_i2c_master_tx(&i2c_handle,
                         SLAVE_ADDRESS_WRITE,
                         &master_write_request,
                         1U);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /* Send the number of bytes that will be written. */
        master_write_request = WRITE_LEN;

        hal_i2c_master_tx(&i2c_handle,
                         SLAVE_ADDRESS_WRITE,
                         &master_write_request,
                         1U);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /* Send the data buffer. */
        hal_i2c_master_tx(&i2c_handle,
                         SLAVE_ADDRESS_WRITE,
                         master_tx_buffer,
                         WRITE_LEN);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /* Send the read command. */
        master_read_request = MASTER_READ_CMD;

        hal_i2c_master_tx(&i2c_handle,
                         SLAVE_ADDRESS_WRITE,
                         &master_read_request,
                         1U);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /* Send the number of bytes requested from the slave. */
        master_read_request = READ_LEN;

        hal_i2c_master_tx(&i2c_handle,
                         SLAVE_ADDRESS_WRITE,
                         &master_read_request,
                         1U);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /* Read the response from the slave. */
        memset(master_rx_buffer, 0, sizeof(master_rx_buffer));

        hal_i2c_master_rx(&i2c_handle,
                         SLAVE_ADDRESS_READ,
                         master_rx_buffer,
                         READ_LEN);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        /*
         * The result can be inspected in the debugger.
         * 0 means the buffers match.
         */
        if (buffer_compare(slave_tx_buffer,
                           master_rx_buffer,
                           READ_LEN) != 0U)
        {
            /* Data mismatch. */
        }
        else
        {
            /* Data received successfully. */
        }

        delay_gen();

#else

        /* Receive a command from the master. */
        hal_i2c_slave_rx(&i2c_handle,
                         &slave_received_command,
                         1U);

        while (i2c_handle.State != HAL_I2C_STATE_READY)
        {
        }

        if (slave_received_command == MASTER_WRITE_CMD)
        {
            /*
             * Receive the write length.
             * Reuse slave_received_command to store the length.
             */
            hal_i2c_slave_rx(&i2c_handle,
                             &slave_received_command,
                             1U);

            while (i2c_handle.State != HAL_I2C_STATE_READY)
            {
            }

            if (slave_received_command <= WRITE_LEN)
            {
                memset(slave_rx_buffer, 0, sizeof(slave_rx_buffer));

                hal_i2c_slave_rx(&i2c_handle,
                                 slave_rx_buffer,
                                 slave_received_command);

                while (i2c_handle.State != HAL_I2C_STATE_READY)
                {
                }

                /*
                 * The result can be inspected in the debugger.
                 * 0 means the buffers match.
                 */
                (void)buffer_compare(slave_rx_buffer,
                                     master_tx_buffer,
                                     slave_received_command);
            }
        }
        else if (slave_received_command == MASTER_READ_CMD)
        {
            /*
             * Receive the requested read length.
             */
            hal_i2c_slave_rx(&i2c_handle,
                             &slave_received_command,
                             1U);

            while (i2c_handle.State != HAL_I2C_STATE_READY)
            {
            }

            if (slave_received_command <= READ_LEN)
            {
                hal_i2c_slave_tx(&i2c_handle,
                                 slave_tx_buffer,
                                 slave_received_command);

                while (i2c_handle.State != HAL_I2C_STATE_READY)
                {
                }
            }
        }

#endif
    }
}

/* -------------------------------------------------------------------------- */
/* STM32F407 interrupt handlers                                               */
/* -------------------------------------------------------------------------- */

void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&i2c_handle);
}

void I2C1_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&i2c_handle);
}
