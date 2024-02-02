#ifndef MAIN_H
#define MAIN_H

typedef struct Vector{
	int x;
	int y;
} Vector;

int w, h;
int steps;
int interval;
volatile int counter;

Vector * cursor;
Vector * anchor;

char * input_file;
char * output_file;
int delta_steps;
char filetype;

volatile int STOP_FLAG;

#endif
