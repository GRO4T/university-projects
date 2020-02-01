#include<ncurses.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>//sin
#include<string.h>//strlen

#include"input.h"

#define DELTA (cursor->x -1)*w + ( cursor->y - anchor->y)

void wait(double msec);

void print_menu(int h);
Vector * menu(char * map, int w, int h, Vector * cursor, Vector * anchor, Vector * game_val);

void map_randomize(char * map, int len);
void map_reset(char * map, int len);
void map_update(char * map, int w, int h, Vector * cursor, Vector * anchor);
void map_draw(char * map, int w, int h, Vector * cursor, Vector * anchor);

void simulate(char * map, int w, int h);

void game(int w, int h);

int main(){
	int w, h = 0; 		
	get_map_dimensions(&w, &h);

	initscr();		//inicjalizacja okna
	raw();  		//wylacz buforowanie liniowe
	keypad(stdscr, TRUE);	//wlacz F1, F2, strzalki...
      	noecho();		//wylacz echo
	
	game(w, h);
	refresh();	//wypisz z bufora do okna
	getch();	//poczekaj na wejscie od uzytkownika
	endwin();	//zakoncz okno

	return(0);
}

void print_menu(int h){
	char * menu[] = { "Press R to randomize the map", "Press G when you're ready to start",
			"Press Q to exit the game"};
	int len = sizeof(menu) / sizeof(char *);
	
	int anchor_x = h + 2;
	int anchor_y;
	for (int i = 0; i < len; ++i){
		anchor_y = (COLS - strlen(menu[i])) / 2;
		mvprintw(anchor_x + i, anchor_y, menu[i]);
	}

	refresh();
}

void map_update(char * map, int w, int h, Vector * cursor, Vector * anchor){
	anchor->x = 0;
	anchor->y = (COLS - w) / 2;	
	
	for(int i = anchor->x + 1; i <= h + anchor->x; ++i){
		for (int j = anchor->y; j < w + anchor->y; ++j){
			char c = *(map + ( (i-anchor->x-1) * w + j-anchor->y));
			if (c == '+'){
				*(map + ( (i-anchor->x-1) * w + j-anchor->y)) = '#';
				c = '#';
			}
			else if (c == '-'){
				*(map + ( (i-anchor->x-1) * w + j-anchor->y)) = ' ';
				c = ' ';	
			}
			mvprintw(i, j, "%c", c);
		}
	}
	move(cursor->x, cursor->y);
}
//narysuj obramowanie planszy
void map_draw(char * map, int w, int h, Vector * cursor, Vector * anchor){
	map_update(map, w, h, cursor, anchor);

	for(int i = anchor->x; i <= h + anchor->x; ++i){			
		if (i == anchor->x)
			for (int j = anchor->y; j < w + anchor->y; ++j)
				mvprintw(i, j, "_");
		else{
			mvprintw(i, anchor->y - 1, "|");
			mvprintw(i, w + anchor->y, "|");
		}
	}
	move(anchor->x + 1, anchor->y);

	cursor->x = anchor->x + 1;
	cursor->y = anchor->y;
}


//zwraca wskaznik game_val kiedy wcisnieto G w przeciwnym razie zwrocony zostaje NULL
Vector * menu(char * map, int w, int h, Vector * cursor, Vector * anchor, Vector * game_val){
	get_game_values(game_val);

	//wyswietl menu i narysuj plansze
	print_menu(h);
	map_draw(map, w, h, cursor, anchor);
	
	int c;
	do{
		c = getch();
		if (c == '\n'){
			if (*(map + DELTA) == ' '){	//ozywianie komorek
				*(map + DELTA) = '#';
				printw("#");
			}
			else{
				*(map + DELTA) = ' ';//zabijanie komorek
				printw(" ");
			}
				move(cursor->x, cursor->y);
		}
		else if(c == 'r' || c == 'R'){
			map_randomize(map, w * h);
			map_update(map, w, h, cursor, anchor);	
		}
		else{	//poruszanie sie kursorem po mapie				
			if (c == KEY_LEFT && cursor->y > anchor->y)
				move(cursor->x, --(cursor->y));
			else if (c == KEY_RIGHT && cursor->y < (w-1) + anchor->y)
				move(cursor->x, ++(cursor->y));
			else if (c == KEY_DOWN && cursor->x < h + anchor->x)
				move(++(cursor->x), cursor->y);
			else if (c == KEY_UP && cursor->x > anchor->x + 1)
				move(--(cursor->x), cursor->y);
		}
	} while (c != 'g' && c != 'G' && c != 'Q' && c != 'q');
	
	if (c == 'g' || c == 'G')
		return game_val;
	else
		return NULL;
		
}
//synchronizuje mapy
void map_reset(char * map, int len){
	for (int i = 0; i < len; ++i)
		*(map + i) = ' ';
}
void map_randomize(char * map, int len){
	int probability = rand() % 50;
	int chance;
	int easing_func;
	for (int i = 0; i < len; ++i){
		chance = sin(i) * (rand() % 100);
		if (chance > 50)
			*(map + i) = '#';
		else
			*(map + i) = ' ';
	}
}
void simulate(char * map, int w, int h){
	int c;
	int moves[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} }; // (1, 1) prawy dolny rog

	for (int i = 0; i < h; ++i){
		for (int j = 0; j < w; ++j){
			c = 0;
			for (int a = 0; a < 8; ++a){
				int dx = i + moves[a][0];
				int dy = j + moves[a][1];
				if ( dx >= 0 && dx < h && dy >= 0 && dy < w){
					if ( *(map + dx*w + dy) == '#' || *(map + dx*w + dy) == '-' )
						c++;
				}
			}		
			if ( *(map + i * w + j) == '#' ){
				if ( c != 2 && c != 3 )
					*(map + i * w + j) = '-';
			}
			else{
				if ( c == 3 )
					*(map + i * w + j) = '+';
			}
		}
	}
}
void wait(double msec){
	clock_t start = clock();
	clock_t end = start + msec*CLOCKS_PER_SEC / 1000.0;

	while (start < end)
		start = clock();
}

void game(int w, int h){
	srand(time(NULL));

	//zainicjalizuj mape
	char * map = malloc(sizeof(char) * w * h);	//tablica do przechowywania aktualnego stanu mapy
	map_reset(map, w * h);	
	
	Vector cursor;
	Vector anchor;
	Vector * game_values = malloc(sizeof(Vector));	

	game_values = menu(map, w, h, &cursor, &anchor, game_values);
	if(game_values != NULL){//jesli wcisnieto 'G'		 

		for (int i = 0; i < game_values->x; ++i){
			wait(game_values->y);
			simulate(map, w, h);
			map_update(map, w, h, &cursor, &anchor);
			mvprintw(0, 0, "%d", i + 1);//wyswietl ilosc krokow symulacji
			refresh();
		}
	}

	free(game_values);
	free(map);
}

