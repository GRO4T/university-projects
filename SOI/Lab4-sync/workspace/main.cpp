#include <stdint.h>

#include "thread_monitor.h"

#define SLEEPTIME 500000

int a1_count = 5;
int b1_count = 5;
int a2_count = 5;
int b2_count = 5;

Buffer buffer;
ThreadMonitor threadMonitor(buffer);

void * a1Producer(void * nr);
void * b1Consumer(void * nr);
void * a2Producer(void * nr);
void * b2Consumer(void * nr);

int main(int argc, char * argv[])
{
    if (argc != 1 && argc != 5){
        printf("Usage:\tprogram_name [a1_count b1_count a2_count b2_count]");
        return -1;
    }
    else if (argc == 5){
        a1_count = atoi(argv[1]);
        b1_count = atoi(argv[2]);
        a2_count = atoi(argv[3]);
        b2_count = atoi(argv[4]);
    }

	int i;
	pthread_t tid[a1_count + b1_count];

    //create A1 producers
	for ( i = 0; i < a1_count; i++)
		pthread_create(&(tid[i]), NULL, a1Producer, (void*)(intptr_t)i );
    //create B1 consumers
	for ( i = a1_count; i < a1_count + b1_count; i++)
		pthread_create(&(tid[i]), NULL, b1Consumer, (void*)(intptr_t)(i-a1_count));
    //create A2 producers
	for ( i = a1_count + b1_count; i < a1_count + b1_count + a2_count; i++)
		pthread_create(&(tid[i]), NULL, a2Producer, (void*)(intptr_t)(i-a1_count-b1_count));
    //create A2 producers
	for ( i = a1_count + b1_count + a2_count; i < a1_count + b1_count + a2_count + b2_count; i++)
		pthread_create(&(tid[i]), NULL, b2Consumer, (void*)(intptr_t)(i-a1_count-b1_count-a2_count));

	//wait for threads termination
	for ( i = 0; i < a1_count + b1_count + a2_count + b2_count; i++)
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
