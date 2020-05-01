#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "a1_proc.h"

int * next_a1_number;

void spawn_process(void (*process_function)(Buffer * buf), Buffer * buf){
    pid_t pid = fork();
    if (pid == 0){
        process_function(buf);
        exit(0);
    }
}

void a1_process(Buffer * buf){
    for (int i = 0; i < 4; ++i){
        int val;
        Buffer__put(buf, val);
        printf("A1 puts %d\n", val);
    }
}
/*
void a2_process(Buffer * buf){
    for (int i = 0; i < 4; ++i){
        int val;
        if (Buffer__even_more_odd(buf)){
            val = Buffer__next_odd_mod_100(buf);
            Buffer__put(buf, val);
        }
        printf("A2 puts %d\n", val);
    }
}

void b1_process(Buffer * buf){
    for (int i = 0; i < 10; ++i){
        if (Buffer__get_size(buf) >= 3){
            if (Buffer__peek(buf) % 2 == 0)
                printf("B1 pops %d\n", Buffer__pop(buf));
        }
    }
}
*/
int * create_shared_int(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(int), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (int *) shmat(shm_id, NULL, 0);
}

int main(){
    //initialize shared variables
    next_a1_number = create_shared_int();

    Buffer * buf = Buffer__bind();
    Buffer__init(buf);

    Proc_factory * proc_factory = Proc_factory__bind();
    Proc_factory__init(proc_factory);
    
    Proc_factory__spawn_b2(proc_factory, buf);
    Proc_factory__spawn_a1(proc_factory, buf);
    Proc_factory__spawn_a2(proc_factory, buf);
    Proc_factory__spawn_b1(proc_factory, buf);

    while (wait(NULL) > 0);
}
