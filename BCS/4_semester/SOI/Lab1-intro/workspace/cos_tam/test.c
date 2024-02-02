#include </usr/include/stdio.h>
#include </usr/include/lib.h>
int getprocnr(int);

int main (int argc, char* argv[]) {
  int i, index;

  if (argc > 1) {
    int pid = atoi(argv[1]);

    for (i=0; i<10; i++) {
      index = getprocnr(pid + i);

      if (index != -1)
         printf("PID: %d : Index: %d\n", pid+i, index);
      else
         printf("Blad: %d. Proces %d nie istnieje.\n", errno, pid+i);
    }
  } else { 
    printf("Brak PID");
  }

  return 0;
}

int getprocnr(int pid) {
  message m;

  m.m1_i1 = pid; 
  return (_syscall(0, 78, &m));
} 
