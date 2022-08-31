#include "sem.h"

void sem_init( volatile SemType *Sem )
{
	(*Sem)=sem_valid;
}

void sem_set( volatile SemType *Sem )
{
	(*Sem)=sem_valid;
}
 //wait the sem into valid
sem_sta  sem_wait( volatile SemType *Sem ) 
{
	if(sem_valid == *Sem)
	{
		*Sem = sem_none;
		return sem_valid;
	}
  	else
		return sem_none;
}
