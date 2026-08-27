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

void hal_i2c_rise_time_configuration(I2C_TypeDef *i2cx,uint32_t freqrange, uint32_t ClockSpeed)
{
	  /*---------------------------- I2Cx TRISE Configuration --------------------*/
  /* Configure I2Cx: Rise Time */
//  hi2c->Instance->TRISE = I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed);

	uint32_t trise;
	if( ClockSpeed <= 100000)
	{
		trise = freqrange +1;
	}else
	{
		trise = (((freqrange * 300) / 1000) + 1);
	}

		i2cx->TRISE  &= ~(0x3F);
		i2cx->TRISE  |= trise;

}

void hal_i2c_clk_init(I2C_TypeDef *i2cx, uint32_t clkspeed, uint32_t duty_cycle)
{
	uint32_t pclk = I2C_PERIPHERAL_CLK_FREQ_8MHZ;
	i2cx->CR2 |= (pclk );
	hal_i2c_rise_time_configuration(i2cx,pclk, clkspeed);
	hal_i2c_configure_ccr(i2cx,pclk,clkspeed,duty_cycle);
}


void hal_i2c_set_addressing_mode(I2C_TypeDef *i2cx, uint32_t adr_mode)
{
	if(adr_mode == I2C_ADDRMODE_10BI)
		i2cx->OAR1 |= I2C_REG_OAR1_ADDRMODE;
	else
		i2cx->OAR1 &= ~I2C_REG_OAR1_ADDRMODE;
}



void hal_i2c_set_fm_mode_duty_cycle(I2C_TypeDef *i2cx, uint32_t duty_cycle)
{
	if(duty_cycle == I2C_FM_DUTY_16BY9 )
	{
     i2cx->CCR |= I2C_REG_CCR_DUTY;
	}else
  {
     i2cx->CCR &= ~I2C_REG_CCR_DUTY;
	}

}


void hal_i2c_manage_ack(I2C_TypeDef *i2cx, uint32_t ack_noack)
{
	if(ack_noack == I2C_ACK_ENABLE)
		i2cx->CR1 |= I2C_REG_CR1_ACK;
	else
		i2cx->CR1 &= ~I2C_REG_CR1_ACK;
}

void hal_i2c_generate_start_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_START_GEN;

}

void hal_i2c_generate_stop_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_STOP_GEN;

}

void hal_i2c_configure_tx_rx_interrupt(I2C_TypeDef *i2cx, uint32_t val)
{
	if(val)
	  i2cx->CR2 |= I2C_REG_CR2_BUF_INT_ENABLE;
	else
		i2cx->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;

}




void hal_i2c_configure_error_interrupt(I2C_TypeDef *i2cx, uint32_t val)
{

	if(val)
	  i2cx->CR2 |= I2C_REG_CR2_ERR_INT_ENABLE;
	else
		i2cx->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;

}
