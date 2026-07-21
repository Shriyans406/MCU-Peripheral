/*
 * ds1307.c
 *
 *  Created on: Jul 19, 2026
 *      Author: HP
 */

#include<stdint.h>
#include<string.h>

#include "ds1307.h"


static void ds1307_i2c_pin_config(void);
I2C_Handle_t g_ds1307I2cHandle;

uint8_t ds1307_init(void)
{
	//1. init the i2c pins
	ds1307_i2c_pin_config();

	//2. initialize the i2c peripheral
	ds1307_i2c_config();

	//3. Enable the I2C peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE);

	//4. Make clock halt = 0;
	ds1307_write(0x00,DS1307_ADDR_SEC);



}

void ds1307_set_current_time(RTC_time_t *rtc_time)
{

}

void ds1307_set_current_date(RTC_date_t *rtc_date)
{

}

void ds1307_get_current_time(RTC_time_t *rtc_time)
{

}

void ds1307_get_current_date(RTC_date_t *rtc_date)
{

}


static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda,i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));


	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_sda);


	i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_scl);

}


void ds1307_i2c_config(void)
{
	g_ds1307I2cHandle.pI2Cx = DS1307_I2C;
	g_ds1307I2cHandle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	g_ds1307I2cHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2cHandle);
}


void ds1307_write(uint8_t value,uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;
	I2C_MasterSendData(&g_ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0);
}
