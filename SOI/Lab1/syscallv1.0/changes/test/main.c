#include <lib.h>
#include <stdlib.h>
#include <stdio.h>
/* whole file <modified> */

int getprocnr(int proc_id){
	message msg;

	msg.m1_i1 = proc_id;
	return (_syscall(MM, GETPROCNR , &msg));
}

int main(int argc, char * argv[]){
	int result;
	int i;

	if (argc == 1)
		printf("Argument needed!\n");
	else{
		int value = atoi(argv[1]);
		for (i=0; i < 10; ++i){
			result = getprocnr(value + i);
			if (result != -1)
				printf("Indeks procesu o pid: %d to %d\n",
				value + i, result);
			else
				printf("Procesu o pid: %d nie ma w tablicy!, Kod bledu %d\n", value + i, errno);
		}
	}

	return 0;
}

