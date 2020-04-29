#ifndef SEMAPHORE_H
#define SEMAPHORE_H

union semun{
    int val;
    struct semid_ds * buf;
    unsigned short int * array;
    struct seminfo *__buf;
};

typedef struct Semaphore{
    int start_value;
    int semid;

    union semun argument;
} Semaphore;

int Semaphore__alloc(Semaphore * self, key_t key, int sem_flags){
    self->semid = semget(key, 1, sem_flags);
    return self->semid;
}

int Semaphore__dealloc(Semaphore * self){
    union semun ignored_argument;
    return semctl(self->semid, 1, IPC_RMID, ignored_argument);
}

int Semaphore__init(Semaphore * self){
    unsigned short values[1];
    values[0] = self->start_value;
    self->argument.array = values;
    return semctl(self->semid, 0, SETALL, self->argument);
}

void Semaphore__create(Semaphore * self, int _start_value){
    self->start_value = _start_value;
}

int Semaphore__wait(Semaphore * self){
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = -1;
    operations[0].sem_flg = SEM_UNDO;

    return semop(self->semid, operations, 1);
}

int Semaphore__post(Semaphore * self){
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = SEM_UNDO;

    return semop(self->semid, operations, 1);
}

#endif
