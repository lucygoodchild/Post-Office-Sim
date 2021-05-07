#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "simQ.h"

int main(int argc, char *argv[])
{
	int	simTime; /*total time of simulation - closingTime + time for customers in queue to be served*/
	int     clock; /*counter for passing time*/
	int     closingTime; /*time of which service points close*/
	int     maxQueueLength; /*parameter - maximum length of queue*/
	int     unfulfilled; /*counter for no. of customers who arrive but queue is full*/
	int     i,j,k,l,m; /*counters*/
	int     timedOut; /*counter for customers whose time in queue is greater than their wait tolerance*/
	int     totalServTime;/*counter for sum of service times of customers currently in queue*/
	int 	numServicePoints; /*parameter for the number of service points open*/
	int     fulfilled; /*counter for the number of customers who are served and leave the post office*/
	int     timeIntervalNo;/*counter for number of time intervals*/
	int 	randServTime; /*assigned to each customers randdom service time*/
	int 	randWaitTol; /*assigned to each customers queue wait tolerance*/
	int 	randCustArr; /*assigned for the random number of customers to arrive in each time interval*/
	int     waitTime; /*counter for total of customers time spent in queue*/
	int     totalSimTime; /*counter for the sum of all sim times*/
	int     totalCustomers; /*counter for total customers that pass through simulations*/
	int     totalToQueue; /*counter for the number of customers to join the queue*/
	int     fulfilledSingle; /*counter for fulfilled customers when numSims == 1*/
	int     timedOutSingle; /*counter for timed-out customers when numSims == 1*/
	int     beingServed;/*counter for no. of customers being served*/
	int     meanCustomerArr; /*parameter for mean no. of customers to arrive in each time intervall*/

/*----------------------------------------------------------------------------------------------*/
	/*file pointers*/
	FILE *fileIn = fileIn = fopen(argv[1], "r");
	FILE *fileOut = fileOut = fopen(argv[3], "w");

	int numSims = atoi(argv[2]);

	if(fileIn && fileOut == NULL)
	{
		fprintf(stderr,"Invalid. Check you have entered inputfile, number of simulations and outputfile.\n");
		exit(-1);
	}
	/*atoi converts NULL to 0 so check for that*/
	if(numSims == 0)
	{
		fprintf(stderr, "Invalid. Check you have entered inputfile, number of simulations and outputfile.\n");
	}
	
	/*reading in parameters from file*/
	fscanf(fileIn, "%*s %d", &maxQueueLength);
	fscanf(fileIn, "%*s %d", &numServicePoints);
	fscanf(fileIn, "%*s %d", &closingTime);
	fscanf(fileIn, "%*s %d", &meanCustomerArr);

	fclose(fileIn); /*close input parameter file*/
	
	fprintf(fileOut, "End of simulation.\n Parameters:\n");
	fprintf(fileOut, "maxQueueLength %d\n", maxQueueLength);
	fprintf(fileOut, "numServicePoints: %d\n", numServicePoints);
	fprintf(fileOut, "closingTime: %d\n", closingTime);
	fprintf(fileOut, "averageNewCustomersPerInterval: %d\n", meanCustomerArr);			
		
	
/*-------------	---------------------------------------------------------------------------------------*/
	
	QUEUE queue;
	CUSTOMER customer;
	SERVICEPOINT servPoint[numServicePoints];
	
	/*setting counters to 0*/
	waitTime = 0;
	totalSimTime = 0;
	unfulfilled = 0;
	fulfilled = 0;
	timedOut = 0;
	beingServed = 0;

	/*set up for random number gerneration*/
	const gsl_rng_type *T = NULL;
	gsl_rng            *r = NULL;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
		
	gsl_rng_set(r,time(0)); /*seed it*/

/*-------------------------------------MAIN SIM------------------------------------------------*/
	
	/*loop each simulation a no. of numSim times*/	
	for(i=1;i<=numSims;i++)
	{	

	initialiseQueue(&queue);/*creating queue and resetting counters for each sim*/

	/*create number of service points given in parameter file*/
	for(j=0;j<numServicePoints;j++)
	{
		createServPoint(&servPoint[j]);
	}
	
	/*setting counters to 0*/
	simTime = 20000; /*placeholder number till end of simulation*/
	clock = 0;
	timeIntervalNo = 0;

		while(clock < simTime) /*start of time interval*/
		{	
			timeIntervalNo++; /*counter for no. of time intervals so far*/
			totalToQueue = 0;
			fulfilledSingle = 0;
			timedOutSingle = 0;
			
			if(clock < closingTime) /*loop runs enqueueing customers, breaks when closingTime is reached*/
			{
				/*generate a random number with mean given in paramter file*/	
				randCustArr = gsl_ran_poisson(r,meanCustomerArr); /*generates a random x number of customers to arrive in time interval*/
				setCustomerArrTime(&queue,&customer,clock); /*set customer arrival time as current time of clock*/
				
				/*loop for adding customer to queeu*/
				for(k=0;k<randCustArr;k++)
				{		
					/*if queue length is not at maximum*/
					if(maxQueueLength > queue.inQueue)
					{	
						randServTime = gsl_ran_gamma(r,2,1) + 2;/*av service time is 2 minutes*/
						randWaitTol = gsl_ran_gamma(r,1,1) + 15; /*av wait tol is 15 minutes*/
						setCustomerWaitTol(&queue,&customer,randWaitTol);/*set each customer a random wait tolerance*/
						setCustomerServTime(&queue,&customer,randServTime);/*set each customer a random time at service point*/
						addCustomer(&queue);
						totalToQueue++;
					}
					
					/*if the queue is at maximum, customer not enqueued and is 'unfulfilled'*/
					else
					{
						unfulfilled++;
					}

				}
				/*remove customer from queue if it is after the first time interval and customers wait tolerance is greater than the no. of time intervals past*/	
				if(clock > 0 & customer.queueWaitTol > timeIntervalNo)
				{							
					removeCustomer(&queue);
					timedOut++;
					timedOutSingle++;
				}	
			}
			/*loop through number of service points*/
			for(l=0;l<numServicePoints;l++)
			{
				
				if(clock >= servPoint[l].timeRemaining && queue.inQueue > 0)/*create a service point if customer has finished service time or first customer*/
				{
					freeServPoint(&servPoint[l]); /*create a free service point*/
					
					if(removeCustomer(&queue)) /*if removing customer from queue is successful*/
					{
						customerAtServPoint(&servPoint[l]); /*note customer is at service point*/
						waitTime += (5 + (clock - getCustomerArrTime(&queue,&customer))); /*set wait time customer spent in queue*/	
						setRemainingServTime(&servPoint[l],(clock + customer.serviceTime));/*set time customer will spend at service point*/
						fulfilled++;
						fulfilledSingle++;
					}	
				}
				
			}
				
			clock += 5;/*add 5 minutes to begin next time interval*/
			
			/*loop for time after closing where all customers have been served*/
			if(clock >= closingTime && queue.inQueue == 0)
			{	
				if(numSims == 1)
				{
					fprintf(fileOut, "\nClosing time has been reached.\n");/*closing time marker*/
				}
				
				simTime = clock; /*while sim loop will break and simTime will be logged as total clock time*/
				totalSimTime += simTime;
			}
			
			if(numSims == 1)
			{	
				/*how many customers are currently at service points during each time interval*/
				if((totalToQueue - fulfilledSingle - timedOutSingle - queue.inQueue) > 0)
				{
					beingServed = (totalToQueue - fulfilledSingle - timedOutSingle  - queue.inQueue);
				}
				else
				{
					beingServed = 0;
				}
				/*to output file*/
				fprintf(fileOut, "\nThe time interval number: %d\n", timeIntervalNo);
				fprintf(fileOut, "The number of customers currently being served: %d\n", beingServed);
				fprintf(fileOut, "The number of people currently in the queue: %d\n", queue.inQueue);
				fprintf(fileOut, "The number of fulfilled customers so far: %d\n", fulfilled);
				fprintf(fileOut, "The number of unfulfilled customer so far: %d\n", unfulfilled);
				fprintf(fileOut, "The number of timed-out customers so far %d\n", timedOut);

			}
		}/*end of time interval*/
			
	}/*end of total simulations*/

	totalCustomers = unfulfilled + fulfilled + timedOut;

	/*output file content for numSims ==1 */
	if(numSims == 1)
	{
		fprintf(fileOut, "The time in minutes since closing time that all remaining customers are served: %d\n", (simTime - closingTime));
		fprintf(fileOut, "Average wait time in minutes for fulfilled customers: %d\n", (waitTime / numSims)/ totalCustomers + 5);
	}
	/*output file content for numSims > 1*/
	if(numSims > 1)
	{
		fprintf(fileOut, "\nThe number of fulfilled customers: %d\n", (fulfilled / numSims));
		fprintf(fileOut, "The number of unfulfilled customers: %d\n", (unfulfilled / numSims));
		fprintf(fileOut, "The number of timed-out customers: %d\n", (timedOut / numSims));
		fprintf(fileOut, "The average waiting time in minutes for fulfilled customers: %d\n", ((waitTime / numSims)/ totalCustomers) + 5);
		fprintf(fileOut, "The time it takes in minutes from closing time until all remaining customers have been served: %d\n", abs((totalSimTime /  numSims) - closingTime));
	}

/*---------------------------------------------------------------------------------------------------------*/
	fclose(fileOut); /*close output file now it has been written to*/
	
	gsl_rng_free(r); /*free memory from random number gen r pointer*/
	r = NULL;
	
	return 0;
}



