#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

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
    sem_t * semaphore = bind_sem();
    sem_init(semaphore, 1, 1);
    
    pid_t child_pid = fork();
    if (child_pid != 0)
        child_pid = fork();
    pid_t my_pid = getpid();
    if (child_pid == (0)){
        for (int i = 0; i < 2; ++i){
            printf("%d lock mutex\n", my_pid);
            sem_wait(semaphore);
            sem_post(semaphore);
            printf("%d unlock mutex\n", my_pid);
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
