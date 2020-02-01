#include<ncurses.h>
#include<string.h>

#include"menu.h"
#include"main.h"
#include"map.h"

#define DELTA (cursor->x - anchor->x - 1)*w + ( cursor->y - anchor->y)

void print_menu(int if_restart){
	char msg1[30];
	char msg2[30];

	if (if_restart){
		strcpy(msg1, "Press R to restart the game");
		strcpy(msg2, "Press G to resume");
	}	
	else{
		strcpy(msg1, "Press R to randomize the map");
		strcpy(msg2, "Press G to start");
	}

	char * menu[] = { msg1, msg2,
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

int menu(char * map){
	clear();	
	
	print_menu(STOP_FLAG);
	map_draw(map);
	mvprintw(0, 0, "%d", counter);
	move(cursor->x, cursor->y);

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
			if (STOP_FLAG){
				for (int i = 0; i < w*h; ++i)
					*(map + i) = ' ';
				
				clear();
				STOP_FLAG = 0;
				print_menu(STOP_FLAG);
				map_draw(map);
				counter = 0;
				mvprintw(0, 0, "%d", counter);
				move(cursor->x, cursor->y);
			}
			else {
				map_randomize(map, w * h);
				map_update(map);
			}
		}
		else if (c != 'g' && c != 'G'){
			move_cursor(c);
		}
	} while (c != 'g' && c != 'G' && c != 'Q' && c != 'q');

	if (c == 'g' || c == 'G')
	{
		clear();
		map_draw(map);
		const char * msg = "Press S to stop";
		mvprintw(h + 2, (COLS - strlen(msg)) / 2, msg);

		return 1;
	}
	else{
		return 0;
	}

}
void move_cursor(int c){
	if (c == KEY_LEFT && cursor->y > anchor->y)
		move(cursor->x, --(cursor->y));
	else if (c == KEY_RIGHT && cursor->y < (w-1) + anchor->y)
		move(cursor->x, ++(cursor->y));
	else if (c == KEY_DOWN && cursor->x < h + anchor->x)
		move(++(cursor->x), cursor->y);
	else if (c == KEY_UP && cursor->x > anchor->x + 1)
		move(--(cursor->x), cursor->y);
}
