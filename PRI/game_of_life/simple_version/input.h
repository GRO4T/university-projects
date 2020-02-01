

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

typedef struct Vector{
	int x;
	int y;
} Vector;

void clear_buffer();
void clear_str(char * str);

int ctod(char * str);
int getint(char * str, int n);

void get_map_dimensions(int * w, int * h);
void get_game_values(Vector * game_val);

#endif
