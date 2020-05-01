#ifndef PROC_FACTORY_H
#define PROC_FACTORY_H
#include "buffer.h"
#include <unistd.h>
#include <assert.h>

#define N_PROCS 4
#define A1 0
#define A2 1
#define B1 2
#define B2 3

typedef struct Proc_factory{
    sem_t condition[N_PROCS];
    sem_t proc_lock[N_PROCS];
    sem_t fill_count;
    sem_t empty_count;

    sem_t mutex_lock;

    sem_t even;
    sem_t odd;

    int a1_next_val;
    sem_t a1_val_lock;
    int a2_next_val;
    sem_t a2_val_lock;
} Proc_factory;

int Proc_factory__get_a1_next_val(Proc_factory * self){
    int val;
    sem_wait(&self->a1_val_lock);
    self->a1_next_val = (self->a1_next_val + 2) % 100;
    val = self->a1_next_val;
    sem_post(&self->a1_val_lock);
    return val;
}

int Proc_factory__get_a2_next_val(Proc_factory * self){
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
    for (int i = 0; i < N_PROCS; ++i){
        sem_init(&self->proc_lock[i], 1, 1);
    }
    sem_init(&self->condition[A1], 1, 10);
    sem_init(&self->condition[A2], 1, 0);
    sem_init(&self->condition[B1], 1, 0);
    sem_init(&self->condition[B2], 1, 0);

    sem_init(&self->fill_count, 1, 0);
    sem_init(&self->empty_count, 1, M);

    sem_init(&self->even, 1, 0);
    sem_init(&self->odd, 1, 0);
    sem_init(&self->mutex_lock, 1, 1);

    sem_init(&self->a1_val_lock, 1, 1);
    sem_init(&self->a2_val_lock, 1, 1);

    self->a1_next_val = -2;
    self->a2_next_val = -1;
}

void wait_not_zero(sem_t * sem){
    int sval;
    sem_getvalue(sem, &sval);
    if (sval > 0)
        sem_wait(sem);
}

void post_if_zero(sem_t * sem){
    int sval;
    sem_getvalue(sem, &sval);
    if (sval == 0)
        sem_post(sem);
}

void Proc_factory__spawn_a1(Proc_factory * self, Buffer * buf){
    pid_t pid = fork();
    if (pid == 0){
        while(1){
            usleep(500000);
            sem_wait(&self->empty_count);
            sem_wait(&self->condition[A1]);

            sem_wait(&self->mutex_lock);

            int val = Proc_factory__get_a1_next_val(self);
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
                post_if_zero(&self->even);
                wait_not_zero(&self->odd);
            }

            sem_post(&self->mutex_lock);
            sem_post(&self->fill_count);
        }
        exit(0);
    }
}

void Proc_factory__spawn_a2(Proc_factory * self, Buffer * buf){
    pid_t pid = fork();
    if (pid == 0){
        while(1){
            usleep(500000);
            sem_wait(&self->empty_count);
            sem_wait(&self->condition[A2]);

            sem_wait(&self->mutex_lock);

            int val = Proc_factory__get_a2_next_val(self);
            Buffer__put(buf, val);
            printf("A2 puts %d\t", val);
            Buffer__print(buf);

            wait_not_zero(&self->condition[A1]);

            if (Buffer__get_size(buf) >= 7)
                sem_post(&self->condition[B2]);

            if (Buffer__get_size(buf) >= 3)
                sem_post(&self->condition[B1]);
            
            if (Buffer__get_size(buf) == 1){
               post_if_zero(&self->odd);
               wait_not_zero(&self->even);
            }

            sem_post(&self->mutex_lock);
            sem_post(&self->fill_count);
        }
        exit(0);
    }
}

void Proc_factory__spawn_b1(Proc_factory * self, Buffer * buf){
    pid_t pid = fork();
    if (pid == 0){
        while(1) {
            usleep(500000);
            sem_wait(&self->fill_count);
            sem_wait(&self->condition[B1]);
            sem_wait(&self->even);

            sem_wait(&self->mutex_lock);
            
            int val = Buffer__pop(buf);
            assert(val % 2 ==0 );
            printf("B1 pops %d\t", val);
            Buffer__print(buf);

            int cur_val = Buffer__peek(buf);
            if (cur_val % 2 == 0){
                post_if_zero(&self->even);
                wait_not_zero(&self->odd);
                wait_not_zero(&self->condition[A2]);
            }
            else{
                post_if_zero(&self->odd);
                wait_not_zero(&self->even);
                if (Buffer__get_num_even(buf) > Buffer__get_num_odd(buf))
                    sem_post(&self->condition[A2]);
            }

            wait_not_zero(&self->condition[B2]);

            if (Buffer__get_size(buf) < 10)
                sem_post(&self->condition[A1]);

            sem_post(&self->mutex_lock);
            sem_post(&self->empty_count);
        }
        exit(0);
    }
}

void Proc_factory__spawn_b2(Proc_factory * self, Buffer * buf){
    pid_t pid = fork();
    if (pid == 0){
        while(1){
            usleep(500000);
            sem_wait(&self->fill_count);
            sem_wait(&self->condition[B2]);
            sem_wait(&self->odd);

            sem_wait(&self->mutex_lock);

            int val = Buffer__pop(buf);
            printf("B2 pops %d\t", val);
            Buffer__print(buf);
            int cur_val = Buffer__peek(buf);
            if (cur_val % 2 == 0){
                post_if_zero(&self->even);
                wait_not_zero(&self->odd);
                wait_not_zero(&self->condition[A2]);
            }
            else{
                post_if_zero(&self->odd);
                wait_not_zero(&self->even);
                if (Buffer__get_num_even(buf) > Buffer__get_num_odd(buf))
                    sem_post(&self->condition[A2]);
            }

            wait_not_zero(&self->condition[B1]);

            if (Buffer__get_size(buf) < 10)
                sem_post(&self->condition[A1]);

            sem_post(&self->mutex_lock);
            sem_post(&self->empty_count);
        }
        exit(0);
    }
}

#endif
