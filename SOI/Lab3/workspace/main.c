#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
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

int main(){/*  
    int semid = binary_semaphore_allocation(IPC_PRIVATE, IPC_CREAT);
    printf("%d", semid);
    binary_semaphore_initialize(semid);
    binary_semaphore_wait(semid);
    printf("after wait\n");
    getSemInfo(semid);
    binary_semaphore_post(semid);
    printf("after post\n");
    getSemInfo(semid);
    */

    Semaphore sem;
    Semaphore__create(&sem, 2);
    int semid = Semaphore__alloc(&sem, IPC_PRIVATE, IPC_CREAT);
    Semaphore__init(&sem);
    Semaphore__wait(&sem);
    getSemInfo(semid);
    Semaphore__post(&sem);
    getSemInfo(semid);
    Semaphore__dealloc(&sem);
}
