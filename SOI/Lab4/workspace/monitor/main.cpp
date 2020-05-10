#include "monitor.h"

#include <stdint.h>
#include <iostream>
#include <queue>

#define CONS_COUNT 20  //liczba konsumentow
#define ELEM_COUNT 1000  //liczba produkowanych elementow

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


Buffer buffer;


void * producent( void * )
{
  	printf( "Producent uruchomiony\n" );

	for( int i = 1; i <= ELEM_COUNT; ++ i )
	{
		buffer.put( i );
		//sleep(1);
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

int main()
{
	int i;
	pthread_t tid[CONS_COUNT+1];

	//utworz watek producenta
	pthread_create(&(tid[0]), NULL, konsument, 0 );

	//utworz watki konsumentow
	for ( i = 1; i <= CONS_COUNT; i++)
		pthread_create(&(tid[i]), NULL, producent, (void*)(intptr_t)i );


	//czekaj na zakonczenie watkow
	for ( i = 0; i < CONS_COUNT + 1; i++)
	  pthread_join(tid[i],(void **)NULL);
	return 0;
}

