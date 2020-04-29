#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buffer.h"

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

int main(){
    Buffer * buf = Buffer__bind();
    Buffer__init(buf);
    
    pid_t child_pid = fork();
    if (child_pid != 0)
        child_pid = fork();
    pid_t my_pid = getpid();
    if (child_pid == (0)){
        for (int i = 0; i < 2; ++i){
            printf("%d put\n", my_pid);
            Buffer__put(buf, my_pid);
            int val = Buffer__get(buf);
            printf("%d get, value = %d\n", my_pid, val);
        }
        exit(0);
    }
    else{/*
        for (int i = 0; i < 2; ++i){
            printf("%d lock mutex\n", child_pid);
            sem_wait(semaphore);
            int semval = 0;
            sem_getvalue(semaphore, &semval);
            printf("sem = %d\n", semval);
            printf("unlock mutex\n");
            sem_post(semaphore);
            sem_getvalue(semaphore, &semval);
            printf("sem = %d\n", semval);
        }*/
        while (wait(NULL) > 0);
    }
    

}
