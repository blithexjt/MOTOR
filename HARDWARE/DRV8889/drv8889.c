#include "drv8889.h"
#include "motor.h"
#include "spi.h"
#include "timer.h"
#include "delay.h"


#define DRV_Delay_H()	delay_us(1000)
#define DRV_Delay_L()      delay_us(1)

void DRV8889_Init()
{

	GPIO_InitTypeDef GPIO_InitStucture;
	RCC_DRV8889_CLOCK();

	GPIO_InitStucture.GPIO_Pin = DIR_pin|Dir_Y_pin;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStucture.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStucture);

	GPIO_InitStucture.GPIO_Pin = nSleep_pin|nSleep_Y_pin;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStucture.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStucture);
	
	GPIO_InitStucture.GPIO_Pin = nFault_X_pin|nFault_Y_pin;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStucture.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStucture);

	GPIO_InitStucture.GPIO_Pin = Drvoff_pin|CS_pin|Drvoff_Y_pin;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStucture.GPIO_OType = GPIO_OType_OD;    
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStucture);

	GPIO_InitStucture.GPIO_Pin = CS_Y_pin;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStucture.GPIO_OType = GPIO_OType_OD;    
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStucture);
	
	SPI3_Init();
	delay_ms(2);

	DRV8889_CS_X_H();
	DRV8889_CS_Y_H();
	DRV_Delay_H();
	DRV8889_SLEEP_X_Dis();
	DRV8889_SLEEP_Y_Dis();
	delay_ms(1);
	DRV8889_EN_X_dis();
	DRV8889_EN_Y_dis();
	delay_ms(1);

	Drv8889_SetMotorMicrostep(Motor_x,MICROSTEP_16);
	Drv8889_SetMotorMicrostep(Motor_y,MICROSTEP_16);

	Drv8889_EnSTLDetection(Motor_x);
	Drv8889_EnSTLDetection(Motor_y);

}


void Drv8889_SetMotorMicrostep (u8 Motor,u8 microstep)
{
    u8 temp1;
	u8 temp2 = microstep;
	temp1 = Drv8889_read( Motor,CTRL3);
	temp2 = temp2 | (temp1 & (0xf0)); 
	Drv8889_write( Motor,CTRL3,temp2);
  	delay_us(1);	
	Set_Fsq_Count(Motor,microstep);
}

void Drv8889_SetCurrent (u8 Motor,u8 current)
{
	u8 temp1 = 0;
	u8 temp2 = (current<<4);
	temp1 = Drv8889_read(Motor,CTRL1);
	temp2 = temp2 | (temp1 & (0x0f)); 
	Drv8889_write(Motor,CTRL1,temp2);
	temp1 = Drv8889_read(Motor,CTRL1);
	delay_us(1);
}



void Drv8889_EnSTLDetection(u8 Motor)
{
  u8 temp1;
	u8 temp2 = STLDetection;
	temp1 = Drv8889_read( Motor,CTRL5);
	temp2 = temp2 |temp1; 
	Drv8889_write( Motor,CTRL5,temp2);
  	delay_us(1);	
}

void Drv8889_DisSTLDetection(u8 Motor)
{
  	u8 temp1;
	u8 temp2 = STLDetection;
	temp1 = Drv8889_read( Motor,CTRL5);
	temp2 = (~temp2) & temp1; 
	Drv8889_write( Motor,CTRL5,temp2);
  	delay_us(1);	
}

u8 Drv8889_ReadSTLStatus(u8 Motor)
{
	return Drv8889_read( Motor,CTRL7);
}


void Drv8889_DisOut (u8 Motor,u8 disout)
{
	u8 temp1 = 0;
	u8 temp2 = (disout<<7);
	temp1 = Drv8889_read(Motor,CTRL2);
	temp2 = temp2 | (temp1 & (0x7f)); 
	Drv8889_write(Motor,CTRL2,temp2);
}


void Drv8889_write(u8 Motor,u8 address,u8 value)
{
	u8 temp = 0;
	u8 temp2 = value;
	temp = (address<<1);
	if(Motor == Motor_x)
	{
		Drv8889_CS_EN(CS_X);
		SPI3_ReadWriteByte(temp);
		SPI3_ReadWriteByte(temp2);
		Drv8889_CS_DIS(CS_X);
	}
	else if(Motor == Motor_y)
	{
		Drv8889_CS_EN(CS_Y);
		SPI3_ReadWriteByte(temp);
		SPI3_ReadWriteByte(temp2);
		Drv8889_CS_DIS(CS_Y);
	}
}



u8 Drv8889_read(u8 Motor,u8 address)
{
	u8 temp = 0;
	u8 value = 0;
	temp = (address<<1) | (0x40);
	if(Motor == Motor_x)
	{
		Drv8889_CS_EN(CS_X);
		SPI3_ReadWriteByte(temp);
		value = SPI3_ReadWriteByte(0xff);
		Drv8889_CS_DIS(CS_X);
	}
		else if(Motor == Motor_y)
	{
		Drv8889_CS_EN(CS_Y);
		SPI3_ReadWriteByte(temp);
		value = SPI3_ReadWriteByte(0xff);
		Drv8889_CS_DIS(CS_Y);
	}
	return value;
}

void Drv8889_CS_EN(u8 CS)
{
	switch(CS)
	{		
		case CS_X:  DRV8889_CS_Y_H();	DRV_Delay_H();DRV8889_CS_X_L();break;
		case CS_Y:	DRV8889_CS_X_H();	DRV_Delay_H();DRV8889_CS_Y_L();break;
	}
	DRV_Delay_L();
}

void Drv8889_CS_DIS(u8 CS)
{
	DRV_Delay_L();
	switch(CS)
	{		
		case CS_X:  DRV8889_CS_X_H();break;
		case CS_Y:	DRV8889_CS_Y_H();break;
	}
	DRV_Delay_H();
}
