/*service points.h*/

#ifndef __SERVICEPOINTS_H_
#define __SERVICEPOINTS_H

#include <stdio.h>

struct servicePoint
{
	unsigned int beingServed;
	int 	     customer;
	int 	     timeRemaining;
};
typedef struct servicePoint SERVICEPOINT;

/*---------------FUNCTION PROTOTYPES--------------------*/
void createServPoint(SERVICEPOINT *);
int freeServPoint(SERVICEPOINT *);
void setRemainingServTime(SERVICEPOINT *, int);
void customerAtServPoint(SERVICEPOINT *);
int getTotalCustomers(SERVICEPOINT *);
int getNoServed(SERVICEPOINT *);

#endif
