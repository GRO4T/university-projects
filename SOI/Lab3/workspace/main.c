#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proc_factory.h"

int main(){
    Buffer * buf = Buffer__bind();
    Buffer__init(buf);

    Proc_factory * proc_factory = Proc_factory__bind();
    Proc_factory__init(proc_factory);

    Proc_factory__spawn_proc(proc_factory, buf, A1);
    Proc_factory__spawn_proc(proc_factory, buf, A1);
    Proc_factory__spawn_proc(proc_factory, buf, A2);
    Proc_factory__spawn_proc(proc_factory, buf, A2);
    Proc_factory__spawn_proc(proc_factory, buf, B2);
    Proc_factory__spawn_proc(proc_factory, buf, B2);
    Proc_factory__spawn_proc(proc_factory, buf, B1);
    Proc_factory__spawn_proc(proc_factory, buf, B1);

    getchar();

    while (wait(NULL) > 0);
}
