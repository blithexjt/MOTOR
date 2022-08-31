#include "motor.h"
#include "timer.h"
#include "drv8889.h"
#include "delay.h"
#include "usart.h"
#include "ucommand.h"


const u32 Fsq[11] = {1,1,2,2,4,8,16,32,64,128,256};
static u8 Fsq_X_Count;
static u8 Fsq_Y_Count;


void MotorInit()
{
	DRV8889_Init();
}

 void MotorSetCurrent(u8 Motor, u8 current)
{
	u8 temp1 = Motor;
	u8 temp2 = current;
	#ifdef DRV8889
	Drv8889_SetCurrent (temp1,temp2);

	#else


	#endif
	
}

void MotorSetSpeed(u8 motor,u8 speed)
{
	u32 arr = 0xffffff;
	u32 freq = 0;
	u32 psc = 1;
	TIM_TypeDef* TIMER;
	
	if(motor == Motor_x)
	{	
		if(0 == speed)
		{
			TIM_CtrlPWMOutputs(TIM4, DISABLE);
			return ;
		}
		freq = speed*200*Fsq[Fsq_X_Count];
		TIMER = TIM4;
	}
	if(motor == Motor_y)   
	{
		if(0 == speed)
		{
			TIM_CtrlPWMOutputs(TIM10, DISABLE);
			return ;
		}
		freq = speed*200*Fsq[Fsq_Y_Count];
		TIMER = TIM10;
	}

	while(arr >= 65535)
	{
		arr = 84000000/(psc+1)/freq - 1;
		psc++;
	}
	psc = psc-1;
	TIM_update(TIMER,arr,psc,AutoUpdate);	
}

void MotorRun(u8 Motor, u32 current,u32 speed, float length)
{
	int num = length*(200*Fsq[Fsq_X_Count]);
	u32 arr = 0xffffff;
	u32 freq = 0;
	u32 psc = 1;
	
	switch(Motor)
	{
		case Motor_x:		
			//TIM_CtrlPWMOutputs(TIM4, ENABLE); 
			num = length*(200*Fsq[Fsq_X_Count]);
			freq = speed*200*Fsq[Fsq_X_Count];
			break;
		case Motor_y:		
			//TIM_CtrlPWMOutputs(TIM10, ENABLE); 
			num = length*(200*Fsq[Fsq_Y_Count]);
			freq = speed*200*Fsq[Fsq_X_Count];
			break;
	}
	MotorSetCurrent( Motor, current);	
	while(arr >= 65535)
	 {
		  arr = 84000000/(psc+1)/freq - 1;
		  psc++;
	 }
	psc = psc-1;
	if(0 == num) return;
	if(num < 0)
	{
		num = -num;
		MoveBackward (Motor,arr,psc,num);
	}
	else
	{
		MoveForward(Motor,arr,psc,num);
	}
}

void MotorStart(u8 Motor,u8 current)
{
		switch(Motor)
		{
			case	Motor_x:
							DRV8889_EN_X_en();
							MotorSetCurrent( Motor, current);
							break;
			case	Motor_y:
							DRV8889_EN_Y_en();
							MotorSetCurrent( Motor, current);
							break;
		}
}


void MotorStop(u8 Motor,u8 current)
{
	switch(Motor)
	{
		case	Motor_x:
							TIM_CtrlPWMOutputs(TIM4, DISABLE);
							TIM_Cmd(TIM4, DISABLE);  
							TIM_Cmd(TIM3, DISABLE);
							break;
		case	Motor_y:
							TIM_CtrlPWMOutputs(TIM10, DISABLE);
							TIM_Cmd(TIM10, DISABLE);  
							TIM_Cmd(TIM9, DISABLE);
							break;
	}
	MotorSetCurrent( Motor, current);
}	


void Set_Fsq_Count(u8 Motor,u8 num)
{
	switch(Motor)
	{
		case	Motor_x:
							Fsq_X_Count = num;
							break;
		case	Motor_y:
							Fsq_Y_Count = num;
							break;
	}
}

void ReadMotorSTL(void)
{
	u8 STL_XStatus =0;
	u8 STL_YStatus =0;
	STL_XStatus = Drv8889_ReadSTLStatus(Motor_x);
	STL_YStatus = Drv8889_ReadSTLStatus(Motor_y);
	printf("the value of MOTORX STL is %d\r\n",STL_XStatus);
	printf("the value of MOTORY STL is %d\r\n",STL_YStatus);
}

void MoveBackward(u8 Motor,u32 arr,u32 psc,u32 step)
{
	u32 pulse = step;
	switch(Motor)
	{
		case Motor_x: 
			MOTORX_DIR_Backward();		
			Pulse_output(Motor_x,arr,psc,pulse);
			break;
		case Motor_y: 
			MOTORY_DIR_Backward();		
			Pulse_output(Motor_y,arr,psc,pulse);
			break;
	}
}

void MoveForward(u8 Motor,u32 arr,u32 psc,u32 step)
{
	u32 pulse = step;
	switch(Motor)
	{
		case Motor_x:  
			MOTORX_DIR_Forward();		
			Pulse_output(Motor_x,arr,psc,pulse);
			break;
		case Motor_y:  
			MOTORY_DIR_Forward();		
			Pulse_output(Motor_y,arr,psc,pulse);
			break;
	}	
}

