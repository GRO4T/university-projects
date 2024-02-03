#include"input.h"
#include<ncurses.h>
#include<math.h>//pow

//konwertuje tablice char na inta
int ctod(char * str){
	char * first = str;
	int num = 0;
	int i = 0;

	do{
		if (*str > 57 || *str < 48)//sprawdz czy to cyfra
			return 0;
		str++;
	} while ( *(str+1) != '\0' );//koncz kiedy nastepny jest znakiem konca tablicy

	do{//konwersja
		str--;
		num +=  pow(10, i) * (*(str) - 48);
		++i;
	} while (str != first);
	
	clear_str(str);

	return num;
}
//pobiera tablice znakow z okna ncurses
int getint(char * str, int n){
	int i = 0;
	char * first = str;
	while ( (*str = getch()) != '\n' && i < n){
		i++;
		str++;
	}
	str = first;

	return ctod(str);
}
//czysci zwykly bufor
void clear_buffer(){
	int c;
	do 
		c = getchar();
	while ( c != '\n' && c != EOF);
}
//czysci tablice
void clear_str(char * str){
	while (*str != '\0')
		*(str++) = 0;
}
//pobiera rozmiar mapy z terminala
void get_map_dimensions(int * w, int * h){
	puts("Give dimensions of the game plane");
	
	printf("width = ");
	while ( 1 != scanf( "%d", w) || *w > 80){
		clear_buffer();			//czyszczenie bufora po kazdym wywolaniu scanf tak aby
						//nie powstala petla nieskonczona
		if (*w > 80)							
			puts("width should be less than 80 units");
		else
			puts("Please, give a number!");
		printf("width = ");
	}
	clear_buffer();				//czyszczenie bufora przed wprowadzeniem kolejnych danych

	printf("height = ");
	while ( 1 != scanf( "%d", h) || *h > 25){
		clear_buffer();

		if (*h > 25)
			puts("height should be less than 25 units");
		else
			puts("Please, give a number!");
		printf("height = ");	
	}
}
//pobiera ilosc krokow i czas miedzy krokami z okna ncurses
void get_game_values(Vector * game_val){
	int x = LINES / 2;
	int y = (COLS - 40) / 2;
	char str[5] = ""; 
	int val;
	echo();

	mvprintw(x, y, "How much turns do you want to simulate?\n");
	do{
		move(x + 1, COLS / 2);
		val = getint(str, 4);
	
		clear();
		if (val > 100){
			val = 0;
			mvprintw(x, y, "Don't you think it's a little bit to much?");
		}
		else if(val == 0)
			mvprintw(x, y, "Please, give a number greater than 0");
		refresh();
	} while (!val);
	game_val->x = val;

	clear();

	val = 0;

	mvprintw(x, y, "Give the interval between the steps(in ms)");
	do{
		move(x + 1, COLS / 2);
		val = getint(str, 5);

		clear();
		if (val > 10000){
			val = 0;
			mvprintw(x, y, "Don't you think it's a little bit to much?");
		}
		else if (val == 0)
			mvprintw(x, y, "Please, give a number greater than 0");
		refresh();
	} while(!val);
	game_val->y = val;

	noecho();
}


