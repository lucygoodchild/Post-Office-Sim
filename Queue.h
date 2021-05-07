/*Queue.h*/

#ifndef __QUEUE_H
#define __QUEUE_H

/*Linked list node to store a queue entry*/
struct QueueNode
{
	int               arrTime;
	int               queueWaitTol;
	int               serviceTime;
	struct QueueNode *nextNode;
};
typedef struct QueueNode CUSTOMER;

/*The queue, head stores the front node and tail the last node of the linked list*/
struct Queue
{
	int       inQueue; /*counter for number of customers in queue*/
	CUSTOMER *head;
	CUSTOMER *tail;
};
typedef struct Queue QUEUE;

/*--------------------------------FUNCTION PROTOTYPES------------------------*/
void initialiseQueue(QUEUE *);
int  emptyQueue(QUEUE *);
void enQueue(QUEUE *, CUSTOMER *);
int  deQueue(QUEUE *, CUSTOMER *);

#endif
