/*Queue.c*/
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

/*function to initialise a new empty queue data structure*/
void initialiseQueue(QUEUE *q)
{
	q->inQueue = 0;
	q->head = NULL;
	q->tail = NULL;
}

/*function to empty queue*/
int emptyQueue(QUEUE *q)
{
	return q->inQueue == 0;
}

/*function to add node to queue*/
void enQueue(QUEUE *q, CUSTOMER *x)
{
	if (q->head == NULL)
		q->head = q->tail = x;
	else
	{
		(q->tail)->nextNode = x;
		q->tail = x;
	}
	q->inQueue++; /*add one to customer in queue counter*/
}
/*function to remove node from queue*/
int deQueue(QUEUE *q, CUSTOMER *x)
{
	CUSTOMER *tmp = NULL;
	
	if(!emptyQueue(q))
	{
		tmp = q->head; /*save to free removed node*/
		*x = *(q->head); /*copy entire structure*/
		q->head = (q->head)->nextNode;
		q->inQueue--; /*minus one from customers in queue counter*/
		if (q->head == NULL)
		{
			q->tail = NULL; /*fix tail if dequeueing last node*/
			return 0;
		}
	}
	else
	{
		return 1;
	}

	tmp = NULL;
	x = NULL;
}
