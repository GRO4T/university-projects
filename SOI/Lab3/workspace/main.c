#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buffer.h"

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
            int val = Buffer__pop(buf);
            printf("%d get, value = %d\n", my_pid, val);
        }
        exit(0);
    }
    else{
        while (wait(NULL) > 0);
    }
    

}
