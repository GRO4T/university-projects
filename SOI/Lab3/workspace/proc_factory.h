#ifndef PROC_FACTORY_H
#define PROC_FACTORY_H
#include "buffer.h"
#include <unistd.h>

#define N_PROCS 4
#define A1 0
#define A2 1
#define B1 2
#define B2 3

#define SLEEPTIME 0

typedef struct Proc_factory{
    sem_t condition[N_PROCS];

    sem_t even;
    sem_t odd;

    int a1_next_val;
    sem_t a1_val_lock;
    int a2_next_val;
    sem_t a2_val_lock;
} Proc_factory;

int _Proc_factory__get_a1_next_val(Proc_factory * self){
    int val;
    sem_wait(&self->a1_val_lock);
    self->a1_next_val = (self->a1_next_val + 2) % 100;
    val = self->a1_next_val;
    sem_post(&self->a1_val_lock);
    return val;
}

int _Proc_factory__get_a2_next_val(Proc_factory * self){
    int val;
    sem_wait(&self->a2_val_lock);
    self->a2_next_val = (self->a2_next_val + 2) % 100;
    val = self->a2_next_val;
    sem_post(&self->a2_val_lock);
    return val;
}

Proc_factory * Proc_factory__bind(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(Proc_factory), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (Proc_factory *) shmat(shm_id, NULL, 0);
}

void Proc_factory__init(Proc_factory * self){
    sem_init(&self->condition[A1], 1, 0);
    sem_init(&self->condition[A2], 1, 0);
    sem_init(&self->condition[B1], 1, 0);
    sem_init(&self->condition[B2], 1, 0);

    sem_init(&self->even, 1, 0);
    sem_init(&self->odd, 1, 0);

    sem_init(&self->a1_val_lock, 1, 1);
    sem_init(&self->a2_val_lock, 1, 1);

    self->a1_next_val = -2;
    self->a2_next_val = -1;
}

void _Proc_factory__A1_job(Proc_factory * self, Buffer * buf){
    while(1){
        usleep(SLEEPTIME);
        while (Buffer__get_size(buf) >= 10)
            sem_wait(&self->condition[A1]);

        int val = _Proc_factory__get_a1_next_val(self);
        Buffer__put(buf, val);
        printf("A1 puts %d\t", val);
        Buffer__print(buf);

        if (Buffer__get_num_even(buf) > Buffer__get_num_odd(buf))
            sem_post(&self->condition[A2]);

        if (Buffer__get_size(buf) >= 7)
            sem_post(&self->condition[B2]);

        if (Buffer__get_size(buf) >= 3){
            sem_post(&self->condition[B1]);
        }

        if (Buffer__get_size(buf) == 1){
            sem_post(&self->even);
        }
    }
}
void _Proc_factory__A2_job(Proc_factory * self, Buffer * buf){
    while(1){
        usleep(SLEEPTIME);
        while (Buffer__get_num_even(buf) <= Buffer__get_num_odd(buf))
            sem_wait(&self->condition[A2]);

        int val = _Proc_factory__get_a2_next_val(self);
        Buffer__put(buf, val);
        printf("A2 puts %d\t", val);
        Buffer__print(buf);

        if (Buffer__get_size(buf) >= 7)
            sem_post(&self->condition[B2]);

        if (Buffer__get_size(buf) >= 3)
            sem_post(&self->condition[B1]);
        
        if (Buffer__get_size(buf) == 1){
           sem_post(&self->odd);
        }
    }
}

void _Proc_factory__B1_job(Proc_factory * self, Buffer * buf){
    while(1) {
        usleep(SLEEPTIME);
        while (Buffer__peek(buf) % 2 != 0)
            sem_wait(&self->even);
        while (Buffer__get_size(buf) < 3)
            sem_wait(&self->condition[B1]);

        int val = Buffer__pop(buf);
        printf("B1 pops %d\t", val);
        Buffer__print(buf);

        int cur_val = Buffer__peek(buf);
        if (cur_val % 2 == 0){
            sem_post(&self->even);
        }
        else{
            sem_post(&self->odd);
            if (Buffer__get_num_even(buf) > Buffer__get_num_odd(buf))
                sem_post(&self->condition[A2]);
        }

        if (Buffer__get_size(buf) < 10)
            sem_post(&self->condition[A1]);
    }
}

void _Proc_factory__B2_job(Proc_factory * self, Buffer * buf){
    while(1){
        usleep(SLEEPTIME);
        while (Buffer__peek(buf) % 2 == 0)
            sem_wait(&self->odd);
        while (Buffer__get_size(buf) < 7)
            sem_wait(&self->condition[B2]);

        int val = Buffer__pop(buf);
        printf("B2 pops %d\t", val);
        Buffer__print(buf);

        int cur_val = Buffer__peek(buf);
        if (cur_val % 2 == 0){
            sem_post(&self->even);
        }
        else{
            sem_post(&self->odd);
            if (Buffer__get_num_even(buf) > Buffer__get_num_odd(buf))
                sem_post(&self->condition[A2]);
        }

        if (Buffer__get_size(buf) < 10)
            sem_post(&self->condition[A1]);
    }
}

void Proc_factory__spawn_proc(Proc_factory * self, Buffer * buf, int p_type){
    void (*job)(Proc_factory * self, Buffer * buf);

    switch(p_type){
        case A1:
            job = _Proc_factory__A1_job; break;
        case A2:
            job = _Proc_factory__A2_job; break;
        case B1:
            job = _Proc_factory__B1_job; break;
        case B2:
            job = _Proc_factory__B2_job; break;
    }

    pid_t pid = fork();
    if (pid == 0){
        job(self, buf);
        exit(0);
    }
}

#endif
