#ifndef BUFFER_H
#define BUFFER_H

#include "monitor.h"
#include <queue>

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
        printf("[");
        while (!temp.empty()){
            printf("%d, ", temp.front());
            temp.pop();
        }
        printf("]\n");
    }

protected:
    int capacity;
    std::queue<int> items;
    int numEven;
    int numOdd;
	Condition emptyCond, fullCond;
};

#endif
