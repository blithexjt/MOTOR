#include "force.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sem.h"
#include	"motor.h"
#include "ucommand.h"
#include <string.h>
#include <stdio.h>
#include "stdlib.h"


struct queue  list;
volatile SemType gsem_event_flag;

const AT_cmd_fun at_cmd_func[] = {	{AT_CMD_SMPR,         "SMPR",          NULL},
									{AT_CMD_SGDM,         "SGDM",          NULL},
									{AT_CMD_GOD,          "GOD",            NULL},
									{AT_CMD_GSD,          "GSD",            NULL},
									{AT_END,              NULL,                NULL}
								};

void M8128_Queue_Init()
{
	M8128_COMMAND  sgdm;	
	initQueue(&list);
	
	sem_init(&gsem_event_flag);

	sgdm.keybuff = "SMPR";
	sgdm.format = set;
	sgdm.val_num = 1;
	sgdm.val[0] = 1000;
	EnQueue(&list, sgdm);
	
	sgdm.keybuff = "SGDM";
	sgdm.format = set;
	sgdm.val_num = 6;
	sgdm.val[0] = 'E';
	sgdm.val[1] =  1;
	sgdm.val[2] =  1;
	sgdm.val[3] =  1;
	sgdm.val[4] =  1;
	sgdm.val[5] =  1;
	EnQueue(&list, sgdm);
	
	sgdm.keybuff = "GSD";
	sgdm.format = getdata;
	EnQueue(&list, sgdm);

}

u8 M8128_Config()
{
	static TIM_Delay  delay;
	M8128_COMMAND  sgdm;
	static command_sta com_sta = tx;
	if(tx == com_sta)
	{	
		DeQueue(&list, &sgdm);
		SendCommand(sgdm);		
		delay_TimInit(&delay,Delay_40_ms,ENABLE);
		com_sta = rx;
	}
	else
	{
		if(sem_valid != sem_wait(&gsem_event_flag))
		{
			delay_TimInit(&delay, Delay_40_ms,DISABLE);
			com_sta = tx;
			return revok;
		}	
		else if(delay_Tim(&delay))
		{
			delay_TimInit(&delay, Delay_40_ms,DISABLE);
			com_sta = tx;
			return timeout;
		}
		return revwait;
	}
	
}


void SendCommand(	M8128_COMMAND  sgdm)
{
	u8 num =0;
	char buff[DMA_USART3_TX_BUF_LEN]={0};
	if(sgdm.format== Read)
	{          
		sprintf(buff,"AT+%s=?\r\n",sgdm.keybuff);
		USART3_SendString(buff);
	}
	else if(sgdm.format== set)
	{ 
		num = sgdm.val_num;
		switch(num)
		{
			case 1:	sprintf(buff,"AT+%s=%d\r\n",sgdm.keybuff,sgdm.val[0]);break;
			case 2:	sprintf(buff,"AT+%s=%d,%d\r\n",sgdm.keybuff,sgdm.val[0],sgdm.val[1]);break;
			case 3:	sprintf(buff,"AT+%s=%d,%d,%d\r\n",sgdm.keybuff,sgdm.val[0],sgdm.val[1],sgdm.val[2]);break;
			case 4:	sprintf(buff,"AT+%s=%d,%d,%d,%d\r\n",sgdm.keybuff,sgdm.val[0],sgdm.val[1],sgdm.val[2],sgdm.val[3]);break;
			case 5:	sprintf(buff,"AT+%s=%d,%d,%d,%d,%d\r\n",sgdm.keybuff,sgdm.val[0],sgdm.val[1],sgdm.val[2],sgdm.val[3],sgdm.val[4]);break;
			case 6:	sprintf(buff,"AT+%s=(A01,A02,A03);%c;1;(WMA:%d)\r\n",sgdm.keybuff,sgdm.val[0],sgdm.val[1]);break;
			default:    break;
		}
		USART3_SendString(buff);
	}
	else if(sgdm.format== getdata)
	{              
		sprintf(buff,"AT+%s\r\n",(u8*)sgdm.keybuff);
		USART3_SendString(buff);
	}
	
}



u8 RevCommnd()
{
	UART3_Receive* uartStruct;
	u8 *rxbuf = NULL;
	char parse_flag = false,i = 0;
	AT_Cmd  atcmd;
	for(i = 0;i<Uart3Array_Len;i++)
	{
		uartStruct = GetUart3Buffer(i);
		if(uartStruct->DataSta == UartData_Vaild)
		{
			//clear the USART3 DataSta flag
			uartStruct->DataSta = UartData_None;
			rxbuf = uartStruct->com3_rx_buffer;
			
			//check Command format
			if((NULL != strstr((const char*)rxbuf,"ACK")) && (NULL != strstr((const char*)rxbuf,"OK")))
			{	
				
				atcmd = AT_cmd_search(&rxbuf[4]);
				if(atcmd != AT_END)
				{
					printf("%s\n",rxbuf);
					printf("%s\n",at_cmd_func[atcmd].str);
				}
				else
				{
					printf("not match command ");
				}
				parse_flag = true;
			}
			//float data format
			else if((rxbuf[0] == 0xAA) && (rxbuf[1] == 0x55))			
			{       
				YGetEng(rxbuf);
				parse_flag = true;
			}
			//error command
			else
				printf("command format error\n");
			

			//check if it  parse successfully
			if(true == parse_flag)  	
			{
				sem_set(&gsem_event_flag);
				return revok;
			}
			else 
				return revwait;
		}
	}
}

void RealTimeMode(bool mode)
{
	// no response back
	if(mode)
	{
		printf("RealTimeMode ON\r\n");
		USART3_SendString("AT+GSD\r\n");
	}
	else
	{
		USART3_SendString("AT+GSD=STOP\r\n");
	}
}


// To get sampling data from M8128
bool YGetEng(u8* com3_rx_ptr)
{
//  structure
//	FrameHeader	PackageLength	DataNo	     Data	     ChkSum/CRC32
//  0xAA,0x55	   HB,LB          2B	(ChNum*N)B/6*4   1B/4B

	u8 *pRxBuffer = NULL;
	int i = 0,j = 0,k = 0,m = 0;
	int Index, Length;
	u8 CheckSum;
	pRxBuffer = (u8*)com3_rx_ptr;
	
	Length = pRxBuffer[i+2]*256 + pRxBuffer[i+3];
	Index = i + 6;
 	 CheckSum = 0;
	for(j = 0; j < M812X_CHN_NUMBER*4; j++)
	{
		CheckSum += pRxBuffer[Index++];
	}
	if(CheckSum !=  pRxBuffer[Index])	return false;
	Index = i + 6;
	for(k = 0; k < M812X_CHN_NUMBER; k++)
	{				
		for(m = 0; m < 4; m++)
		{
			DealDataBufferUnion[k].m_ByteResultDataBuffer[m] = pRxBuffer[Index++];
		}
		m_dEngValue[k] = DealDataBufferUnion[k].m_FloatResultData;
	}
	printf("the capture data is %f  %f  %f \r\n", m_dEngValue[0] ,m_dEngValue[1] ,m_dEngValue[2]  );
	return true;
}

 AT_Cmd AT_cmd_search(u8 *p)
{
    unsigned char ret = 0;
    unsigned char i, n;
    for(i=0; at_cmd_func[i].cmd != AT_END; i++)
     {    
        
        n = strlen((const char*)at_cmd_func[i].str);
        if(!strncmp((const char*)p, (const char*)at_cmd_func[i].str, n))
        {
            ret = i;
            break;
        }        
    }
    return ret;
}
