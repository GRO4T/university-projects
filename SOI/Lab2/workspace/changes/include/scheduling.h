#include <lib.h>

int getpri(pid_t pid, int ppri_type){
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = ppri_type;
	return _syscall(MM, GETPRI, &m);
}

int setpri(pid_t pid, int ppri_type, int value){
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = ppri_type;
	m.m1_i3 = value;
	return _syscall(MM, SETPRI, &m);
}
