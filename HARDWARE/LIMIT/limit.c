#include "limit.h"
#include "motor.h"
#include "usart.h"
#include "drv8889.h"

void LIMIT_Init(void)
{
	GPIO_InitTypeDef      GPIO_Initstructure;
	RCC_LIMIT_CLOCK();


	GPIO_Initstructure.GPIO_Pin = LIMIT1_PIN|LIMIT2_PIN|LIMIT3_PIN|LIMIT4_PIN;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( LIMIT_PORT, &GPIO_Initstructure );

	GPIO_ResetBits(LIMIT_PORT,  LIMIT1_PIN|LIMIT2_PIN|LIMIT3_PIN|LIMIT4_PIN);
}

void EXTIX_Init(void)
{
	NVIC_InitTypeDef     NVIC_InitStructure;
	EXTI_InitTypeDef      EXTI_InitStructure;

	LIMIT_Init();
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG,  ENABLE);
	
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource2);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource3);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource4);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource5);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2|EXTI_Line3|EXTI_Line4|EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




void EXTI2_IRQHandler(void)
{
	if(0 == LIMIT1)
	{
		MotorStop( Motor_x, CURRENT_25_PERCENT);
		printf("limit");
	}
	EXTI_ClearITPendingBit( EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	if(0 == LIMIT2)
	{
		MotorStop( Motor_x, CURRENT_25_PERCENT);
		printf("limit");
	}
	EXTI_ClearITPendingBit( EXTI_Line3);
}
void EXTI4_IRQHandler(void)
{
	if(0 == LIMIT3)
	{
		MotorStop( Motor_y, CURRENT_25_PERCENT);
		printf("limit");
	}
	EXTI_ClearITPendingBit( EXTI_Line4);
}
void EXTI9_5_IRQHandler(void)
{
	if(0 == LIMIT4)
	{
		MotorStop( Motor_x, CURRENT_25_PERCENT);
		printf("limit");
	}
	EXTI_ClearITPendingBit( EXTI_Line5);
}
