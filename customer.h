/*customer.h*/

#ifndef __CUSTOMER_H
#define __CUSTOMER_H

#include <stdio.h>
#include "Queue.h"

/*------------FUNCTION PROTOTYPES--------------------*/
void addCustomer(QUEUE *);
int setCustomerArrTime(QUEUE *, CUSTOMER *, int);
int setCustomerWaitTol(QUEUE *, CUSTOMER *, int);
int setCustomerServTime(QUEUE *, CUSTOMER *, int);
int removeCustomer(QUEUE *);
int getCustomerArrTime(QUEUE *, CUSTOMER *);

#endif
