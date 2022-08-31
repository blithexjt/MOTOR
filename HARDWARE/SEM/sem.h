#ifndef __SEM_H
#define __SEM_H

#include "sys.h"

typedef u8 SemType;

typedef enum
{
	sem_none,
	sem_valid,
}sem_sta;

/**
  * @brief 	 initial sem to valid.
  * @param 	 Sem: SemType*.
  * @retval 	 None
  */
void sem_init( volatile SemType *Sem );
/**
  * @brief  	set the sem to valid.
  * @param 	Sem: SemType*.
  * @retval 	None
  */
void sem_set( volatile SemType *Sem );
/**
  * @brief        wait the sem into valid.
  * @param      Sem: SemType*.
  * @retval       sem_sta:check if sem is valid
  */
sem_sta  sem_wait( volatile SemType *Sem ) ;

#endif
