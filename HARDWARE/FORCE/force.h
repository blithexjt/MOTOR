#ifndef __FORCE_H
#define __FORCE_H
#include "sys.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "delay.h"
#include "usart.h"
#include "queue.h"


typedef  u8  byte;

#define RX_BUFFER_SIZE	20
#define M812X_CHN_NUMBER	3

typedef unsigned char(*pFunc)(unsigned char *ptr,unsigned char len);

//tx  or rx result status
typedef enum
{
	revok,
	revwait,
	timeout,
	txwait,
}TxRvState;

// at command list
typedef enum
{
	AT_CMD_SMPR=0,
	AT_CMD_SGDM,
	AT_CMD_GOD,
	AT_CMD_GSD,
	AT_END,
}AT_Cmd;

//at command status
typedef enum
{
	rx=0,
	tx,
}command_sta;

//at command format
typedef enum
{
	 Read=1,
	 set,
	 getdata,
}AT_Command_Format;


typedef struct
{
	AT_Cmd cmd;
	unsigned char *str;
	pFunc cb;
}AT_cmd_fun;

//float num transformat
union DealDataFloat2Byte_TypeDef
{
	float m_FloatResultData;
	u8	  m_ByteResultDataBuffer[4];
};

static union DealDataFloat2Byte_TypeDef DealDataBufferUnion[M812X_CHN_NUMBER];
static double m_dEngValue[M812X_CHN_NUMBER];


u8 M8128_Config(void);

void M8128_Queue_Init(void);

u8 RevCommnd(void);

void RealTimeMode(bool mode);
/**
  * @brief 	 Transmit AT command.
  * @param  	 sgdm: m8128 command struct.
  * @retval 	 NONE
  */
void SendCommand(	M8128_COMMAND  sgdm);
/**
  * @brief 	 To get sampling data from M8128.
  * @param  	 com3_rx_ptr: com3 uart buffer.
  * @retval 	 check deal status
  */
bool YGetEng(u8* com3_rx_ptr);

//search command from the command list 
/**
  * @brief 	 search command from the command list .
  * @param  	 p: at command str.
  * @retval 	 check search status
  */
AT_Cmd  AT_cmd_search( u8 *p);

#endif
