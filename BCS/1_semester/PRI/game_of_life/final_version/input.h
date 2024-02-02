#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int clear_buffer();
int check_buffer(char * buf, int len);

void get_value(int * val, int min, int max, const char * msg, char * val_name);
int ask(const char * question);

void parse_args(int argc, char * argv[]);
void get_int_from_file(FILE * file, int * val, int len, int min, int max, const char * msg_nn, const char * msg_tl, char * val_name);
void get_map(FILE * file, char * map);
void open_file(FILE ** file, const char * mode);

void get_game_values(char ** map, int argc, char * argv[]);

int check_extension(char * filename);

#endif
