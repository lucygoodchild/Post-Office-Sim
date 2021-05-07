/*customer.c*/

#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "customer.h"

void addCustomer(QUEUE *q)
{
	CUSTOMER *newCustomer = NULL;
	newCustomer = (CUSTOMER *)malloc(sizeof(CUSTOMER));
	if (newCustomer == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	
	enQueue(q,newCustomer);
	free(newCustomer);
	newCustomer = NULL;
}

int setCustomerArrTime(QUEUE *q, CUSTOMER *c, int clock)
{
	c->arrTime = clock;
	return 0;
}

int setCustomerWaitTol(QUEUE *q, CUSTOMER *c, int randWaitTol)
{ 	
	c->queueWaitTol = randWaitTol;
	return 0;
}

int  setCustomerServTime(QUEUE *q, CUSTOMER *c, int randServTime)
{
	c->serviceTime = randServTime;
	return 0;
}

int removeCustomer(QUEUE *q)
{
	CUSTOMER *customer = NULL;
	customer = (CUSTOMER *)malloc(sizeof(CUSTOMER));
	if(customer == NULL)
	{
		fprintf(stderr,"Out of memory\n");
		exit(1);
	}

	deQueue(q,customer);
	free(customer);
	customer = NULL;
	return 1;
}

int getCustomerArrTime(QUEUE *q, CUSTOMER *c)
{
	return c->arrTime;
}
