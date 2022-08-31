#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_conf.h"
#include "queue.h"
#include "sys.h" 	

#define USART_MAX_LEN	200

#define DMA_USART1_TX_BUF_LEN		200
#define DMA_USART3_TX_BUF_LEN		50
#define Uart3Array_Len	5

extern volatile uint16_t com1_rx_len ;    
extern volatile uint8_t com1_recv_end_flag ;
extern u8 com1_rx_buffer[DMA_USART1_TX_BUF_LEN];  


typedef enum
{
	UartData_None = 0,
	UartData_Vaild,
}_eUartData_Sta;

typedef enum
{
	com1_recv_end,
	com1_recv_noend,
}com1_recv_end_state;

typedef enum
{
	RX_Failed,
	RX_Success,
}RX_STA;


typedef struct
{
	u8 com3_rx_buffer[DMA_USART3_TX_BUF_LEN];
	u16 com3_rx_len ;
	_eUartData_Sta DataSta; 
}UART3_Receive;



void uart1_Init(u32 bound);
void DMA_USART1_Send(u8 *data,u16 size);
void USART1_Receive_En(u8 rev_en);

void uart3_Init(u32 bound);
void DMA_USART3_Send(u8 *data,u16 size);
void USART3_Receive_En(u8 rev_en);

void USART3_SendString( u8 *DataString);
UART3_Receive* GetUart3Buffer(u8 revid);

#endif
