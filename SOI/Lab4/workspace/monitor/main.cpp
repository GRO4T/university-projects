#include "monitor.h"

#include <stdint.h>
#include <iostream>
#include <queue>

#define CONS_COUNT 20  //liczba konsumentow
#define ELEM_COUNT 1000  //liczba produkowanych elementow

#define A1_COUNT 5
#define B1_COUNT 5

#define SLEEPTIME 500000

//Bufor jednoelementowy
class Buffer: public Monitor
{
public:
	Buffer() : capacity(20){ }

	void put( int item )
	{
		enter();

		if(items.size() >= capacity)
			wait( emptyCond );

        items.push(item);

        if (item % 2 == 0)
            numEven++;
        else
            numOdd++;

		signal( fullCond );

		leave();
	}

	int get()
	{
		enter();

		int res;

		if (items.empty())
			wait( fullCond );

		res = items.front();
        items.pop();

        if (res % 2 == 0)
            numEven--;
        else
            numOdd--;

		signal( emptyCond );

		leave();

		return res;
	}

    int peek(){
        enter();
        int res;
        res = items.front();
        leave();
        return res;
    }

    int getNumEven(){
        int temp;
        enter();
        temp = numEven;
        leave();
        return temp;
    }

    int getNumOdd(){
        int temp;
        enter();
        temp = numOdd;
        leave();
        return temp;

    }

    int getSize(){
        int temp;
        enter();
        temp = items.size();
        leave();
        return temp;
    }

    void print(){
        std::queue<int> temp = items;
        std::cout << temp.size();
        std::cout << "[";
        while (!temp.empty()){
            std::cout << temp.front() << ", ";
            temp.pop();
        }
        std::cout << "]\n";
    }

protected:
    int capacity;
    std::queue<int> items;
    int numEven;
    int numOdd;
	Condition emptyCond, fullCond;
};

class ThreadMonitor : public Monitor{
    public:
        ThreadMonitor(Buffer & buf) : buffer(buf), nextEven(-2), nextOdd(-1){}

        int putEven(int nr){
            enter();
            int temp;
            if (buffer.getSize() >= 10)
                wait(a1Cond);
            nextEven = (nextEven + 2) % 100;
            temp = nextEven;
            buffer.put(nextEven);

            printf("A1 producer nr:%d %d\t", nr, nextEven);
            buffer.print();
            
            if (buffer.getSize() >= 3 && buffer.peek() % 2 == 0)
                signal(b1Cond);

            leave();
            return temp;
        }

        int getEven(){
            enter();
            int temp;
            if (buffer.getSize() < 3 || buffer.peek() % 2 != 0)
                wait(b1Cond);
            temp = buffer.get();

            if (buffer.getSize() < 10)
                signal(a1Cond);

            leave();
            return temp;
        }
        void checkLess10(){

        }

        void checkLeast3(){

        }

        void checkLeast7(){

        }
    protected:
        Condition a1Cond, a2Cond, b1Cond, b2Cond;
        Buffer & buffer;
        int nextEven;
        int nextOdd;
};

Buffer buffer;
ThreadMonitor threadMonitor(buffer);

void * producent( void * )
{
  	printf( "Producent uruchomiony\n" );

	for( int i = 1; i <= ELEM_COUNT; ++ i )
	{
		buffer.put( i );
	}

	//aby zakonczycz konsumentow
	for( int i = 0; i < CONS_COUNT; i ++ )
		buffer.put( 0 );

	return 0;
}

 void * konsument( void * nr )
{
	int el;
	
	printf( "Konsument %d uruchomiony\n", nr );
	do
	{
		el = buffer.get();
        sleep(1);
		printf( "Konsument nr:%d %d\t", nr, el );
        buffer.print();
	}
	while( el != 0 );

	return 0;
}

void * a1Producer(void * nr){
	for( int i = 1; i <= ELEM_COUNT; ++ i )
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
        val = threadMonitor.getEven();
        printf("B1 Consumer nr:%d %d\n", nr, val);
    }

    return 0;
}

int main()
{
	int i;
	pthread_t tid[A1_COUNT + B1_COUNT];

	//utworz watek producenta
	//pthread_create(&(tid[0]), NULL, konsument, 0 );

	//utworz watki producentów A1
	for ( i = 0; i < A1_COUNT; i++)
		pthread_create(&(tid[i]), NULL, a1Producer, (void*)(intptr_t)i );

    //utwórz wątki konsumentów B1
	for ( i = A1_COUNT; i < A1_COUNT + B1_COUNT; i++)
		pthread_create(&(tid[i]), NULL, b1Consumer, (void*)(intptr_t)(i-A1_COUNT));


	//czekaj na zakonczenie watkow
	for ( i = 0; i < A1_COUNT; i++)
	  pthread_join(tid[i],(void **)NULL);
	return 0;
}

