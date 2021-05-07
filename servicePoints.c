/*servicepoints.c*/

#include <stdio.h>
#include <stdlib.h>
#include "servicePoints.h"

void createServPoint(SERVICEPOINT *s)
{
	s->timeRemaining = 0;
	s->customer = 0;
	s->beingServed = 0;
}

int freeServPoint(SERVICEPOINT *s)
{
	s->beingServed--;
	return s->timeRemaining == 0;
}

void setRemainingServTime(SERVICEPOINT *s, int duration)
{
	s->timeRemaining = duration;
}

void customerAtServPoint(SERVICEPOINT *s)
{
	s->customer++;
	s->beingServed++;
}

int getTotalCustomers(SERVICEPOINT *s)
{
	return s->customer;
}

int getNoServed(SERVICEPOINT *s)
{
	return s->beingServed;
}
