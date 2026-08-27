#include "hal_i2c_driver.h"
#include "led.h"

void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_ENABLE_I2C;
}

void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 &= ~I2C_REG_CR1_ENABLE_I2C;

}



void hal_i2c_set_own_address1(I2C_TypeDef *i2cx, uint32_t own_address)
{
	i2cx->OAR1 &= ~( 0x7f << 1);
	i2cx->OAR1 |=  (own_address << 1);
}


void hal_i2c_manage_clock_stretch(I2C_TypeDef *i2cx, uint32_t no_stretch)
{
	if(no_stretch)
	{
		i2cx->CR1 |= I2C_REG_CR1_NOSTRETCH;
	}else
	{
		i2cx->CR1 &= ~I2C_REG_CR1_NOSTRETCH;
	}
}


void hal_i2c_configure_ccr(I2C_TypeDef *i2cx, uint32_t pclk, uint32_t clkspeed, uint32_t duty_cycle)
{


	double Thigh, Tlow,  Tpclk;
	uint32_t ccr;
	if(clkspeed <= 100000)
	{
		/* configure ccr for standard mode */
		ccr = ( pclk * 1000000) / (clkspeed << 1);

	}else
	{
		 if(duty_cycle == I2C_FM_DUTY_2)
		 {
			 ccr = ( pclk * 1000000)/(3 * clkspeed);

		 }

		  if(duty_cycle == I2C_FM_DUTY_16BY9)
		 {
			 /* this is to reach 400khz in fm mode */
			ccr = ( pclk * 1000000)/(25 * clkspeed);

		 }
	}

	i2cx->CCR |= ccr;

}
