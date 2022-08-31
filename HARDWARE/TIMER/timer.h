#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define CYCLE_1000  		1000
#define PULSENUM_400 	400
#define PULSENUM_MAX	60000

#define TIME_PRESCALER_168  		(168-1)
#define MAIN_FREQUENCE_168M   168000000
#define Motor_Step_Port 	GPIOB     
#define RCC_Motor1_Step  RCC_APB1Periph_TIM4 
#define RCC_Motor2_Step  RCC_APB2Periph_TIM10 

#define Motor1_Step_Pin 	GPIO_Pin_6
#define Motor2_Step_Pin 	GPIO_Pin_8


typedef enum
{
	Motor_x = 0,
	Motor_y,
}MOTOR_NUM;

typedef enum
{
    AutoUpdate = 0,
    immediately,
}TIM_Update;

void RGBLED_Init(void);

void OUTPUT_Init(void);


void TIM1_PWM_Init(void);
void TIM6_Init(u16 arr,u16 psc);

void TIM2_Int_Init(u16 arr,u16 psc);


void TIM3_PWM_Init(u32 PulseNum1);
void TIM4_PWM_Init(u32 arr,u32 psc);


void TIM9_PWM_Init(u32 PulseNum2);
void TIM10_PWM_Init(u32 arr ,u32 psc);

void Pulse_output(u16 num,u32 arr, u32 psc,u32 PulseNum);

void TIM4_Master_TIM3_Slave(u32 PulseNum1,u32 cycle,u32 psc);
void TIM10_Master_TIM9_Slave(u32 PulseNum2,u32 cycle,u32 psc);

void TIM3_IRQHandler(void);
void TIM1_BRK_TIM9_IRQHandler(void);


void TIM_update(TIM_TypeDef * TIMx,u16 arr,u16 psc,u8 option);

void PWM_Out(u32 baut);
void TimerX_Relaod(u16 count);
void TimerY_Relaod(u16 count);



#endif
