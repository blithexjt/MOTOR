#ifndef __UCOMMAND_H
#define __UCOMMAND_H
#include "sys.h"
#include "usart.h"
#include "motor.h"
#include "force.h"

//MOTOR uart command status
typedef enum
{
	MOTORX_Current = 1,
	MOTORX_Speed,
	MOTORX_Start,
	MOTORX_Stop,
	MOTORX_Run,
	MOTORY_Current,
	MOTORY_Speed,
	MOTORY_Start,
	MOTORY_Stop,
	MOTORY_Run,
}UART_COMMAND;

typedef enum
{
	RX_END_Failed,
	RX_END_Success,
}RX_END_STA;

//tcp uart command status
typedef enum
{
	tcp_senddata,
	tcp_quitsend,
	tcp_serve_mode,
	reserve,
}tcp_sta;

typedef struct
{
	double m_dEngValue[M812X_CHN_NUMBER];
	u8 MotorX_STL_Status;
	u8 MotorY_STL_Status;
}Debug_Paramet;

//MOTOR debug usart1
u8 CheckUartCommand(void);

//get MOTOR command paramet
u8 GetUartParameter(char *pInstr,double *pdBuffe,u8 ParametNum);

//printf MOTOR STALL value
void Debug_printf(void);

//tcp serve mode debug usart1
tcp_sta tcp_debug(void);



#endif
