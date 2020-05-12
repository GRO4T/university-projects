#include <stdint.h>

#include "thread_monitor.h"

#define A1_COUNT 5
#define B1_COUNT 5
#define A2_COUNT 5
#define B2_COUNT 5

#define SLEEPTIME 0

Buffer buffer;
ThreadMonitor threadMonitor(buffer);

void * a1Producer(void * nr);
void * b1Consumer(void * nr);
void * a2Producer(void * nr);
void * b2Consumer(void * nr);

int main()
{
	int i;
	pthread_t tid[A1_COUNT + B1_COUNT];

    //create A1 producers
	for ( i = 0; i < A1_COUNT; i++)
		pthread_create(&(tid[i]), NULL, a1Producer, (void*)(intptr_t)i );
    //create B1 consumers
	for ( i = A1_COUNT; i < A1_COUNT + B1_COUNT; i++)
		pthread_create(&(tid[i]), NULL, b1Consumer, (void*)(intptr_t)(i-A1_COUNT));
    //create A2 producers
	for ( i = A1_COUNT + B1_COUNT; i < A1_COUNT + B1_COUNT + A2_COUNT; i++)
		pthread_create(&(tid[i]), NULL, a2Producer, (void*)(intptr_t)(i-A1_COUNT-B1_COUNT));
    //create A2 producers
	for ( i = A1_COUNT + B1_COUNT + A2_COUNT; i < A1_COUNT + B1_COUNT + A2_COUNT + B2_COUNT; i++)
		pthread_create(&(tid[i]), NULL, b2Consumer, (void*)(intptr_t)(i-A1_COUNT-B1_COUNT-A2_COUNT));

	//wait for threads termination
	for ( i = 0; i < A1_COUNT + B1_COUNT + A2_COUNT + B2_COUNT; i++)
	    pthread_join(tid[i],(void **)NULL);

	return 0;
}

void * a1Producer(void * nr){
    while(true)
	{
        usleep(SLEEPTIME);
		threadMonitor.putEven((intptr_t)nr);
	}

    return 0;
}

void * b1Consumer(void * nr){
    int val;
    while (true){
        usleep(SLEEPTIME);
        threadMonitor.getEven((intptr_t)nr);
    }

    return 0;
}

void * a2Producer(void * nr){
    while(true)
	{
        usleep(SLEEPTIME);
		threadMonitor.putOdd((intptr_t)nr);
	}

    return 0;
}

void * b2Consumer(void * nr){
    while (true){
        usleep(SLEEPTIME);
        threadMonitor.getOdd((intptr_t)nr);
    }

    return 0;
}
