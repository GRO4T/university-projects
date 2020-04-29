#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct Buffer{
    sem_t semaphore;
    int data;

} Buffer;
/*  
sem_t * bind_sem(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(sem_t), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (sem_t *) shmat(shm_id, NULL, 0);
}
*/

Buffer * Buffer__bind(){
    //semaphore = bind_sem();
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
}

void Buffer__put(Buffer * self, int value){
    sem_wait(&self->semaphore);
    self->data = value;
    sem_post(&self->semaphore);
}

int Buffer__get(Buffer * self){
    int temp;
    sem_wait(&self->semaphore);
    temp = self->data;
    sem_post(&self->semaphore);
    return temp;
}



#endif
