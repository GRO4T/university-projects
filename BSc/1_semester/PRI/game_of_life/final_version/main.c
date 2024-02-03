#include<ncurses.h>
#include<pthread.h>
#include<time.h>

#include"menu.h"
#include"input.h"
#include"main.h"
#include"map.h"

void free_globals();
void wait(double msec);

void init_vals(char ** map, int argc, char * argv[]);
void init_screen();

void dw_output(FILE ** file);
void * game(void * map);

void * check_for_stop();

int main(int argc, char *argv[]){
	srand(time(NULL));

	char * map = NULL;

	pthread_t game_t;
	pthread_attr_t attr;

	while (1)
	{	
		init_vals(&map, argc, argv);		

		pthread_attr_init(&attr);
		pthread_create(&game_t, &attr, game, (void*)map); 
		pthread_join(game_t, NULL);
		endwin();

		free(map);
		free_globals();

		if (ask("Do you really want to quit? If no the game will be restarted"))
			break;
	}

	return(0);
}
void wait(double msec){
	clock_t start = clock();
	clock_t end = start + msec*CLOCKS_PER_SEC / 1000.0;

	while (start < end)
		start = clock();
}
void * game(void * map){
	pthread_attr_t attr;
	pthread_t cfi_t;

	FILE * file = NULL;

	dw_output(&file);
	init_screen();

	while ( menu(map) ){
		STOP_FLAG = 0;		
		pthread_attr_init(&attr);
		pthread_create(&cfi_t, &attr, check_for_stop, NULL); 

		for (counter; (counter < steps) && !STOP_FLAG; ++counter){
			wait(interval);
			simulate(map);
			map_update(map);
			
			mvprintw(0, 0, "%d", counter);
			move(cursor->x, cursor->y);
			
			refresh();

			if (file != NULL && !(counter % delta_steps)){	
				if (filetype == 't'){
					save_to_txt(file, map);
				}
				else if (filetype == 'h'){	
					save_to_html(file, map);
				}
			}
		}
		STOP_FLAG = 1;
		if (counter >= steps)
			counter = 0;
	}
	if (file)
		fclose(file);

	return NULL;
}
void init_vals(char ** map, int argc, char * argv[]){
	w = h = steps = interval = 0;
	anchor = malloc(sizeof(Vector));
	cursor = malloc(sizeof(Vector));
	STOP_FLAG  = counter = 0;

	delta_steps = 0;
	input_file = NULL;
	output_file = NULL;

	get_game_values(map, argc, argv);
}
void * check_for_stop(){
	fd_set s_rd, s_wr, s_ex;

	char c;
	do{
		FD_ZERO(&s_rd);
		FD_ZERO(&s_wr);
		FD_ZERO(&s_ex);
		FD_SET(fileno(stdin), &s_rd);
		select(fileno(stdin) + 1, &s_rd, &s_wr, &s_ex, NULL);
		c = getchar();
	} while(!STOP_FLAG && (c != 's' && c != 'S'));

	STOP_FLAG = 1;
	return NULL;
}
void free_globals(){
	free(anchor);
	free(cursor);
	if (input_file)
		free(input_file);
	if (output_file)
		free(output_file);
}
void init_screen(){
	initscr();		
	raw();  		//wylacz buforowanie liniowe
	keypad(stdscr, TRUE);	//wlacz F1, F2, strzalki...
	noecho();		//wylacz echo

	anchor->x = 0;
	anchor->y = (COLS - w) / 2;
	cursor->x = anchor->x + 1;
	cursor->y = anchor->y;
}
void dw_output(FILE ** file){
	if (ask("Do you want to generate output file?")){
		if (output_file == NULL){	
			if (ask("Do you want to specify your own name?\n(default is output.txt) ")){
				char filename[30];
				do{
					printf("filename = ");	
					scanf("%29s", filename);
				} while(!check_extension(filename));

				output_file = malloc( (strlen(filename) + 1) * sizeof(char) );
				strcpy(output_file, filename);	
			}
			else{
				output_file = malloc(11 * sizeof(char));
				strcpy(output_file, "output.txt");
				filetype = 't';	
			}
		}

		if (delta_steps == 0)
			get_value(&delta_steps, 0, 10000, "Number of turns between which progress will be saved", "delta steps");

		*file = fopen(output_file, "w");
	}	
}
