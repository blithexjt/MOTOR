#include <stdio.h>
#include "queue.h"

void initQueue(Queue* pq)
{
    if(NULL!=pq)
    {
        pq->front=0;
        pq->rear=0;
    }
}

bool isFull(Queue* pq)
{
    if((pq->rear+1)%MAXSIZE == pq->front)
        return true;    
    return false;
}   
//????
bool isEmpty(Queue* pq)
{

    if(pq->rear==pq->front)
        return true;
    return false;
}
//??:??????
bool EnQueue(Queue*pq,QueueType data)
{
    if(isFull(pq))
        return false;
    //????--rear??
    pq->buf[ pq->rear ]=data; 
    pq->rear=(pq->rear+1)%MAXSIZE;
    return true;
}
//????
unsigned short QueueLength(Queue* pq)
{
    return (pq->rear - pq->front+MAXSIZE)%MAXSIZE;
}
//??:?????
bool DeQueue(Queue* pq,QueueType* data)
{
//?????:
    if(NULL==pq || isEmpty(pq))
        return false;
    *data=pq->buf[pq->front];
    pq->front=(pq->front+1)%MAXSIZE;
    return true;
}

