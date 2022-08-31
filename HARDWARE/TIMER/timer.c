#include "sys.h"
#include "usart.h"	
#include "delay.h"
#include "timer.h"
#include "motor.h"
#include "drv8889.h"


 u32 CurrentNum_x;
 u32 CurrentNum_y;


void RGBLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
 	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12\
								                |GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}


void TIM1_PWM_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能定时器1时钟

	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_Prescaler = TIME_PRESCALER_168;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM1,  TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,  TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,  TIM_OCPreload_Enable);

	TIM_Cmd(TIM1,  ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}



void TIM6_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);	 

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_Cmd(TIM6,  ENABLE);
}



void OUTPUT_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
    
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB7复用为定时器4   
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10); //GPIOA0复用为定时器5
    
	GPIO_InitStructure.GPIO_Pin = Motor1_Step_Pin; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_Step_Port, &GPIO_InitStructure);//初始化GPIO

	GPIO_InitStructure.GPIO_Pin = Motor2_Step_Pin; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_Step_Port, &GPIO_InitStructure);//初始化GPIOA
}



void TIM4_PWM_Init(u32 arr,u32 psc)
{  

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_Motor1_Step, ENABLE);	//使能定时器4时钟
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =  psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = ((arr+1)/2);//占空比为50%
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器

	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);//TIM4为主定时器
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);//
	TIM_ARRPreloadConfig(TIM4, ENABLE);
 
	 
	
}



void TIM3_PWM_Init(u32 PulseNum1)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 
  //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = PulseNum1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Regular);
	TIM_SelectInputTrigger(TIM3,TIM_TS_ITR3);
	TIM3->SMCR|=0x07;
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);
}





void TIM10_PWM_Init(u32 arr ,u32 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);	//使能定时器10
 
   //初始化TIM10
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM10 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = arr/2;
	TIM_OC1Init(TIM10, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1

	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器

	TIM_SelectMasterSlaveMode(TIM10, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM10, TIM_TRGOSource_Update);
	TIM_ARRPreloadConfig(TIM10, ENABLE);
 
	TIM_Cmd(TIM10, ENABLE);  
	
}
void TIM9_PWM_Init(u32 PulseNum2)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);	 

	TIM_TimeBaseStructure.TIM_Period = PulseNum2; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_UpdateRequestConfig(TIM9,TIM_UpdateSource_Regular);
	TIM_SelectInputTrigger(TIM9,TIM_TS_ITR2);
	TIM9->SMCR|=0x07;
	TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SelectOnePulseMode(TIM9,TIM_OPMode_Single);
}


void Pulse_output(u16 MotorNum,u32 arr,u32 psc,u32 PulseNum)
{
	switch (MotorNum)
	{
		case Motor_x:
			TIM4_Master_TIM3_Slave(PulseNum,arr,psc);
			break;
		case Motor_y:
			TIM10_Master_TIM9_Slave(PulseNum,arr,psc);
			break;
	}	
}
void TIM4_Master_TIM3_Slave(u32 PulseNum1,u32 arr,u32 psc)
{
	CurrentNum_x = PulseNum1;
	if(PulseNum1 > PULSENUM_MAX)	
		PulseNum1 = PULSENUM_MAX;
	CurrentNum_x =CurrentNum_x - PulseNum1;
    TIM3_PWM_Init(PulseNum1);
    
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM4_PWM_Init( arr, psc);
    TIM_CtrlPWMOutputs(TIM4, ENABLE);   
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE); 
}


void TIM10_Master_TIM9_Slave(u32 PulseNum2,u32 arr,u32 psc)
{
	CurrentNum_y = PulseNum2;
	if(PulseNum2 > PULSENUM_MAX)	
		PulseNum2 = PULSENUM_MAX;
	CurrentNum_y =CurrentNum_y - PulseNum2;
	TIM9_PWM_Init(PulseNum2);
	TIM_Cmd(TIM9, ENABLE);
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	TIM10_PWM_Init(arr,psc);
	TIM_CtrlPWMOutputs(TIM10, ENABLE);   
	
}

void TimerY_Relaod(u16 count)
{ 
	TIM_SetAutoreload(TIM9,count);   //N为预设PWM个数
	TIM_GenerateEvent(TIM9,TIM_EventSource_Update); 
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE); 
	TIM_Cmd(TIM9, ENABLE); 
}

void TimerX_Relaod(u16 count)
{ 
	TIM_SetAutoreload(TIM3,count);   //N为预设PWM个数
	TIM_GenerateEvent(TIM3,TIM_EventSource_Update); 
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	TIM_Cmd(TIM3, ENABLE); 
}

//update the  frequence of motor
void TIM_update(TIM_TypeDef * TIMx,u16 arr,u16 psc,u8 option)
{
	u16 ccr = (arr/2);
	TIM_UpdateDisableConfig(TIMx, ENABLE);
	TIMx->CCR1= ccr;	
	TIMx->ARR = arr;
	TIM_UpdateDisableConfig(TIMx, DISABLE);	
	delay_ms(1);
	
	TIM_PrescalerConfig( TIMx,  psc,  TIM_PSCReloadMode_Update);
	if(option == immediately)  
		TIM_GenerateEvent(TIMx, TIM_EventSource_Update);
	TIM_Cmd(TIMx,  ENABLE);

}

void TIM1_BRK_TIM9_IRQHandler(void) 
{ 
	if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)     // TIM_IT_CC1
	{
		TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		if(CurrentNum_y)
		{			
			if(CurrentNum_y > PULSENUM_MAX)
			{
				CurrentNum_y -= PULSENUM_MAX;
				TimerY_Relaod(PULSENUM_MAX);
			}
			else
			{
				TimerY_Relaod(CurrentNum_y);
				CurrentNum_y = 0;
			}
		}
		else
		{
	        TIM_ClearITPendingBit(TIM9, TIM_IT_Update); // 清除中断标志位 
		//TIM_CtrlPWMOutputs(TIM10, DISABLE);  
	        TIM_Cmd(TIM10, DISABLE); // 关闭定时器 
	        TIM_Cmd(TIM9, DISABLE); // 关闭定时器 
     		}   
	}
}


void TIM3_IRQHandler(void) 
{ 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)     // TIM_IT_CC1
	{ 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(CurrentNum_x)
		{			
			if(CurrentNum_x > PULSENUM_MAX)
			{
				CurrentNum_x -= PULSENUM_MAX;
				TimerX_Relaod(PULSENUM_MAX);
			}
			else
			{
				TimerX_Relaod(CurrentNum_x);
				CurrentNum_x = 0;
			}
		}
		else
		{
			//TIM_CtrlPWMOutputs(TIM4, DISABLE);  
			TIM_Cmd(TIM4, DISABLE);  
			TIM_Cmd(TIM3, DISABLE); 
			TIM_SetCounter(TIM3,0);
		}
	} 		
}



