#ifndef __LIMIT_H
#define __LIMIT_H
#include "sys.h"

#define  LIMIT_PORT       GPIOE

#define  LIMIT1_PIN	     GPIO_Pin_2
#define  LIMIT2_PIN	     GPIO_Pin_3
#define  LIMIT3_PIN	     GPIO_Pin_4
#define  LIMIT4_PIN	     GPIO_Pin_5

#define  LIMIT1               GPIO_ReadInputDataBit(  LIMIT_PORT,  LIMIT1_PIN)
#define  LIMIT2               GPIO_ReadInputDataBit(  LIMIT_PORT,  LIMIT2_PIN)
#define  LIMIT3               GPIO_ReadInputDataBit(  LIMIT_PORT,  LIMIT3_PIN)
#define  LIMIT4               GPIO_ReadInputDataBit(  LIMIT_PORT,  LIMIT4_PIN)

#define  RCC_LIMIT_CLOCK()       RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE,  ENABLE)

void LIMIT_Init(void);

void EXTIX_Init(void);
#endif
