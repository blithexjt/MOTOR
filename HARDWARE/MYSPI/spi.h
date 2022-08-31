#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

typedef enum 
{
	Mode0 = 0,
	Mode1,
	Mode2,
	Mode3,
}eSPI_Mode;
 
void SPI2_Init(void); 
u8 SPI2_ReadWriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI_Mode(eSPI_Mode mode,SPI_InitTypeDef* SPI_InitStructure);
			  	    													  

void SPI1_Init(void) ;
u8 SPI1_ReadWriteByte(u8 TxData);


void SPI3_Init(void);
u8 SPI3_ReadWriteByte(u8 TxData);




#endif

