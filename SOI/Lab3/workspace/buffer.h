#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <limits.h>

#define M 100
#define NUMBER_OF_PROC 2
#define A1 0
#define B1 1

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

    sem_t even_sem;
    int number_of_even;
    
    sem_t condition_sem[NUMBER_OF_PROC];
    sem_t proc_sem[NUMBER_OF_PROC];
    int next_even_mod_100;
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

    sem_init(&self->even_sem, 1, 1);
    self->number_of_even = 0;

    for (int i = 0; i < NUMBER_OF_PROC; ++i){
        sem_init(&self->proc_sem[i], 1, 1);
        sem_init(&self->condition_sem[i], 1, 1);
    }
    self->next_even_mod_100 = -2;

    sem_wait(&self->condition_sem[B1]);
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

        if (value % 2 == 0){
            self->number_of_even++;
            if (self->number_of_even == 10)
                sem_wait(&self->condition_sem[A1]);
        }

        if (self->size == 3)
            sem_post(&self->condition_sem[B1]);
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

        if (temp % 2 == 0){
            self->number_of_even--;
            if (self->number_of_even == 9)
                sem_post(&self->condition_sem[A1]);
        }

        if (self->size == 2)
            sem_wait(&self->condition_sem[B1]);
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
    return temp; }

int Buffer__less_than_10_even(Buffer * self){
    sem_wait(&self->condition_sem[A1]);
    sem_post(&self->condition_sem[A1]);
    return 1;
}

int Buffer__next_even_mod_100(Buffer * self){
    sem_wait(&self->proc_sem[A1]);
    self->next_even_mod_100 = (self->next_even_mod_100 + 2) % 100;
    sem_post(&self->proc_sem[A1]);
    return self->next_even_mod_100;
}

int Buffer__at_least_3(Buffer * self){
    sem_wait(&self->condition_sem[B1]);
    sem_post(&self->condition_sem[B1]);
    return 1;
}

#endif
