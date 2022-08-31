#include "ucommand.h"
#include <string.h>
#include <stdio.h>
#include "force.h"
#include "usart.h"


extern  u8 com1_rx_buffer[200]; 
extern volatile u8 com1_recv_end_flag;


const double Current[16] = {100,93.75,87.5,81.25,75,68.75,62.5,56.25,50,43.75,37.5,31.25,25,18.75,12.5,6.25};

u8 CheckUartCommand()
{
	double pdBuffer[3] ={0};
	if(RX_END_Success == com1_recv_end_flag)
	{
		//clear flag
		com1_recv_end_flag = RX_END_Failed;
		if(strstr((char*)com1_rx_buffer,"MOTORX-C:"))
		{
			GetUartParameter("MOTORX-C:",pdBuffer,1);
			printf("MOTORX-C:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorSetCurrent( Motor_x,  (u8)pdBuffer[0] );
			return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORX-S:"))
		{
			GetUartParameter("MOTORX-S:",pdBuffer,1);
			printf("MOTORX-S:RECEIVE:%fmm/s\r\n",pdBuffer[0]);
			MotorSetSpeed( Motor_x,  (u8)pdBuffer[0]);
			return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORX-START:"))
		{
			GetUartParameter("MOTORX-START:",pdBuffer,1);
			printf("MOTORX-START:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorStart( Motor_x, (u8)pdBuffer[0]);			 
			 return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORX-STOP:"))
		{
			GetUartParameter("MOTORX-STOP:",pdBuffer,1);
			printf("MOTORX-STOP:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorStop( Motor_x, (u8)pdBuffer[0]);
			return MOTORX_Stop;

		}
		else if(strstr((char*)com1_rx_buffer,"MOTORX-RUN:"))
		{
			GetUartParameter("MOTORX-RUN:",pdBuffer,3);
			printf("MOTORX-RUN:RECEIVE:%f,%f mm/s,%f mm\r\n",Current[(u8) pdBuffer[0]],pdBuffer[1],(pdBuffer[2]/(10.0)));
			MotorRun( Motor_x,  (u8) pdBuffer[0],pdBuffer[1], (pdBuffer[2]/(10.0)));
			return MOTORX_Run;
		}
		else	if(strstr((char*)com1_rx_buffer,"MOTORY-C:"))
		{
			GetUartParameter("MOTORY-C:",pdBuffer,1);
			printf("MOTORY-C:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorSetCurrent( Motor_y,  (u8)pdBuffer[0] );
			return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORY-S:"))
		{
			GetUartParameter("MOTORY-S:",pdBuffer,1);
			printf("MOTORY-S:RECEIVE:%fmm/s\r\n",pdBuffer[0]);
			MotorSetSpeed( Motor_y,  pdBuffer[0]);
			return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORY-START:"))
		{
			GetUartParameter("MOTORY-START:",pdBuffer,1);
			printf("MOTORY-START:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorStart( Motor_y, (u8)pdBuffer[0]);			 
			 return 1;
		}
		else if(strstr((char*)com1_rx_buffer,"MOTORY-STOP:"))
		{
			GetUartParameter("MOTORY-STOP:",pdBuffer,1);
			printf("MOTORY-STOP:RECEIVE:%f\r\n",Current[(u8) pdBuffer[0]]);
			MotorStop( Motor_y, (u8)pdBuffer[0]);
			return MOTORX_Stop;

		}
		else if(strstr((char*)com1_rx_buffer,"MOTORY-RUN:"))
		{
			GetUartParameter("MOTORY-RUN:",pdBuffer,3);
			printf("MOTORY-RUN:RECEIVE:%f,%f mm/s,%f mm\r\n",Current[(u8) pdBuffer[0]],pdBuffer[1],(pdBuffer[2]/(10.0)));
			MotorRun( Motor_y,  (u8) pdBuffer[0],pdBuffer[1], (pdBuffer[2]/(10.0)));
			return MOTORX_Run;
		}
		else  return 0;
	}
	return 0;
}

u8 GetUartParameter(char *pInstr,double *pdBuffe,u8 ParametNum)
{
	char *pIndexBuffer = NULL;
	char CharTemp[16];
	double pdTemp[5];
	
	int k = 0x00;
	int i = 0x00;
	double dTemp = 0.0;
	long int len = strlen(pInstr);
	pIndexBuffer = strstr((char*)com1_rx_buffer,pInstr);
	if(pIndexBuffer == NULL) return false;

	pIndexBuffer = pIndexBuffer+len;

	for( i = 0; i < ParametNum; i++)
	{
		memset(CharTemp,0x00,16);		
		k = 0x00;
		while(*pIndexBuffer != ';' && *pIndexBuffer != '\r'&&*pIndexBuffer != '\n')
		{
			if(((*pIndexBuffer >= 0x30)&&(*pIndexBuffer <= 0x39))||(*pIndexBuffer == '.')||(*pIndexBuffer == '-'))
				CharTemp[k++] = *pIndexBuffer++;		
			else 
			{
				pIndexBuffer++;
				continue;
			}
		}
		if(sscanf(CharTemp,"%lf",&dTemp) != SUCCESS) return 0;	
		pdTemp[i] = dTemp;
		pIndexBuffer++;
	}
	for(i = 0; i < ParametNum; i++)
		pdBuffe[i] = pdTemp[i];
	return 1;
}

void Debug_printf(void)
{
	static TIM_Delay*  delay_stall;
	static u8 flag = 0;
	if(flag)
	{
		flag = 1;
		delay_TimInit(delay_stall,Delay_20_ms,ENABLE);
	}
	else
	{
		if(delay_Tim(delay_stall))
		{
			delay_TimInit(delay_stall, Delay_20_ms,DISABLE);
			ReadMotorSTL();	
		}
	}
	
}

