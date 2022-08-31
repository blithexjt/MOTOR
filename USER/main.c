#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "spi.h"
#include "drv8889.h"
#include "ucommand.h"
#include "motor.h"
#include "force.h"
#include "queue.h"
#include "limit.h"






 int main(void)
 {	
	delay_init(168);
 	delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 	 
	uart1_Init(115200);	
	uart3_Init(115200);  
	OUTPUT_Init();
	TIM6_Init(CYCLE_1000 ,TIME_PRESCALER_168);
	MotorInit();
	M8128_Queue_Init();
	EXTIX_Init();
	 
	
 	while(1)
	{
		//USART MOTOR 
		CheckUartCommand();
		// USART FORCE
		M8128_Config();	
		// FORCE receive and paramet
		RevCommnd();
		//printf MOTOR STALL VALUE
		Debug_printf();	
	}
}


