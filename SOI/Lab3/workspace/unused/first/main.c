#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "semaphore.h"

/*
 * ipcs -s -> to show used semaphores
 * ipcrm sem <semid> -> remove semaphore with semid
 *
 * ipcs -i <semid> -s -> show semaphore details
 *
 */

void getSemInfo(int semid){
    char command[50];
    sprintf(command, "ipcs -s -i %d", semid);
    system(command);
}

Semaphore * bind_sem(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(struct Semaphore), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (Semaphore *) shmat(shm_id, NULL, 0);
}

int main(){

    Semaphore * sem = bind_sem();
    Semaphore__create(sem, 1);
    int semid = Semaphore__alloc(sem, IPC_PRIVATE, IPC_CREAT);
    Semaphore__init(sem);
    pid_t child_pid = fork();
    printf("%d", semid);
    if (child_pid == 0){
        sleep(100);
        Semaphore__wait(sem);
        printf("child in sem\n");
        Semaphore__post(sem);

    }
    else{
        Semaphore__wait(sem);
        printf("parent in sem\n");
        Semaphore__post(sem);
        sleep(1000);
        printf("deallocating\n");
        Semaphore__dealloc(sem);
    }

}
