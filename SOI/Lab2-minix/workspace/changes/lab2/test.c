#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <scheduling.h>
#include <sys/types.h>
#include <time.h>

#define SET_BASE_PRI 0
#define SET_ACT_PRI 1
#define SET_PTYPE 2

#define GET_BASE_PRI 0
#define GET_ACT_PRI 1
#define GET_PTYPE 2
#define GET_PREV_ACT_PRI 3

#define MAX_AGE 1000
#define MIN_PRI 100

int main(int argc, char ** argv){
	int my_pid, i, j, ptype, priority, iterations;
	my_pid = getpid();
	iterations = 5;

	if (argc < 2 || argc > 4){
		printf("Usage:\n\t program_name ptype [priority/burst_time] [iterations]\n");
		return 1;
	}

	ptype = atoi(argv[1]);
	if (ptype < 1 || ptype > 3){
		printf("Not a valid ptype. Valid ptype = {1, 2, 3}\n");
		return 1;
	}
	setpri(my_pid, SET_PTYPE, ptype);

	if (argc >= 3){
		priority = atoi(argv[2]);
		if (ptype == 2 && (priority < MIN_PRI || priority > MAX_AGE)){
			printf("For this process priority must be in range<MIN_PRI, MAX_AGE>\n");
			return 1;
		}
		printf("pid = %d, priority = %d\n", my_pid, priority);
		setpri(my_pid, SET_ACT_PRI, priority);
	}

	if (argc == 4){
		iterations = atoi(argv[3]);
		if (iterations <= 0){
			printf("Iterations should be a positive integer\n");
			return 1;
		}	
	}

	for (i = 0; i < iterations; ++i){
		printf("I'm working, pid = %d(pri = %d) (ptype = %d)\n", my_pid, getpri(my_pid, 3), getpri(my_pid, 2));
		for (j = 0; j < 200; ++j);
	}

	printf("Pid = %d finished\n", my_pid);
	return 0;
}
