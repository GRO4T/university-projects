#ifndef THREAD_MONITOR_H
#define THREAD_MONITOR_H

#include "buffer.h"

class ThreadMonitor : public Monitor{
    public:
        ThreadMonitor(Buffer & buf) : buffer(buf), nextEven(-2), nextOdd(-1){}

        void putEven(int nr){
            enter();
            if (buffer.getSize() >= 10)
                wait(a1Cond);

            nextEven = (nextEven + 2) % 100;
            buffer.put(nextEven);

            printf("A1 producer nr:%d %d\t", nr, nextEven);
            buffer.print();
            
            if (buffer.getSize() >= 3 && buffer.peek() % 2 == 0)
                signal(b1Cond);
            if (buffer.getSize() >=7 && buffer.peek() % 2 == 1)
                signal(b2Cond);
            if (buffer.getNumEven() > buffer.getNumOdd())
                signal(a2Cond);

            leave();
        }

        void getEven(int nr){
            enter();

            if (buffer.getSize() < 3 || buffer.peek() % 2 == 1)
                wait(b1Cond);

            int val = buffer.get();

            printf("B1 consumer nr:%d %d\t", nr, val);
            buffer.print();

            if (buffer.getSize() < 10)
                signal(a1Cond);
            if (buffer.getSize() >= 3 && buffer.peek() % 2 == 0)
                signal(b1Cond);
            if (buffer.getSize() >=7 && buffer.peek() % 2 == 1)
                signal(b2Cond);

            leave();
        }

        void putOdd(int nr){
            enter();

            if (buffer.getNumEven() <= buffer.getNumOdd())
                wait(a2Cond);

            nextOdd = (nextOdd + 2) % 100;
            buffer.put(nextOdd);

            printf("A2 producer nr:%d %d\t", nr, nextOdd);
            buffer.print();

            if (buffer.getSize() >= 3 && buffer.peek() % 2 == 0)
                signal(b1Cond);
            if (buffer.getSize() >=7 && buffer.peek() % 2 == 1)
                signal(b2Cond);

            leave();
        }

        void getOdd(int nr){
            enter();

            if (buffer.getSize() < 7 || buffer.peek() % 2 == 0)
                wait(b2Cond);
            
            int val = buffer.get();

            printf("B2 consumer nr:%d %d\t", nr, val);
            buffer.print();

            if (buffer.getSize() < 10)
                signal(a1Cond);
            if (buffer.getNumEven() > buffer.getNumOdd())
                signal(a2Cond);
            if (buffer.getSize() >= 3 && buffer.peek() % 2 == 0)
                signal(b1Cond);
            if (buffer.getSize() >= 7 && buffer.peek() % 2 == 1)
                signal(b2Cond);

            leave();
        }
    protected:
        Condition a1Cond, a2Cond, b1Cond, b2Cond;
        Buffer & buffer;
        int nextEven;
        int nextOdd;
};

#endif
