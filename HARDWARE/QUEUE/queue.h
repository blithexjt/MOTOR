#ifndef _QUEUE_DEF_
#define _QUEUE_DEF_
#include "sys.h"
#include <stdbool.h>

#define MAXSIZE 10
#define COMMENT_MAX   6


struct queue;
typedef  struct 
{
	char* keybuff;
	u8   format;
	u8   revstate;
	u8   val_num;
	u32  val[COMMENT_MAX];
}M8128_COMMAND;


typedef  M8128_COMMAND  QueueType;


//?????
struct queue
{
    QueueType buf[MAXSIZE];//????
    int rear;//??
    int front;//??
};
//???struct queue;
typedef struct queue Queue;

extern void initQueue(Queue*);//?????
extern bool EnQueue(Queue*,QueueType);//??
extern bool DeQueue(Queue*,QueueType*);//??
extern bool isFull(Queue*);//???
extern bool isEmpty(Queue*);//???
extern unsigned short QueueLength(Queue*);//????
extern bool ClearQueue(void);//???? 

#endif
