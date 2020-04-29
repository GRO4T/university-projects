#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <limits.h>

#define M 10

typedef struct Node{
    int data;
    struct Node * next;
} Node;

typedef struct Buffer{
    sem_t semaphore;
    int data[M];
    int size;
    int headId;
    int tailId;
} Buffer;

Buffer * Buffer__bind(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(Buffer), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (Buffer *) shmat(shm_id, NULL, 0);
}

void Buffer__init(Buffer * self){
    sem_init(&self->semaphore, 1, 1);
    self->headId = self->size = 0;
    self->tailId = M - 1;
}

int Buffer__is_full(Buffer * self){
    return (self->size == M);
}

int Buffer__is_empty(Buffer * self){
    return (self->size == 0);
}

void Buffer__put(Buffer * self, int value){
    sem_wait(&self->semaphore);
    if (Buffer__is_full(self) == 0){
        self->tailId = (self->tailId + 1) % M;
        self->data[self->tailId] = value;
        self->size++;
    }
    sem_post(&self->semaphore);
}

int Buffer__pop(Buffer * self){
    int temp;
    sem_wait(&self->semaphore);
    if (Buffer__is_empty(self))
        temp = INT_MIN;
    else{
        temp = self->data[self->headId];
        self->headId = (self->headId + 1) % M;
        self->size--;
    }
    sem_post(&self->semaphore);
    return temp;
}

int Buffer__peek(Buffer * self){
    int temp;
    sem_wait(&self->semaphore);
    if (Buffer__is_empty(self))
        temp = INT_MIN;
    else{
        temp = self->data[self->headId];
    }
    sem_post(&self->semaphore);
    return temp;
}



#endif
