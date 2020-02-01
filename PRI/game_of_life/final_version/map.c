#include<math.h>
#include<ncurses.h>
#include<stdlib.h>

#include"main.h"

void map_update(char * map){
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

void map_draw(char * map){
	map_update(map);

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
}

void map_randomize(char * map, int len){
	int chance;
	for (int i = 0; i < len; ++i){
		chance = sin(i) * (rand() % 100);
		if (chance > 50)
			*(map + i) = '#';
		else
			*(map + i) = ' ';
	}
}

void simulate(char * map){
	int c;
	int moves[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} }; // (1, 1) prawy dolny rog

	for (int i = 0; i < h; ++i){
		for (int j = 0; j < w; ++j){
			c = 0;
			for (int a = 0; a < 8; ++a){
				int dx = (h + i + moves[a][0]) % h;
				int dy = (w + j + moves[a][1]) % w;
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
void save_to_txt(FILE * file, char * map){
	fprintf(file, "%d\n", counter);
	fprintf(file, " ");
	for (int i = 0; i < w; ++i)
		fprintf(file, "_");
	fprintf(file, "\n");	

	for (int i = 0; i < h; ++i){
		fprintf(file, "|");
		for (int j = 0; j < w; ++j){
			fprintf(file, "%c", *(map + i*w + j));
		}
		fprintf(file, "|\n");
	}
}
void save_to_html(FILE * file, char * map){
	fprintf(file, "<!DOCKTYPE html><html><body>");
	fprintf(file, "%d", counter);
	fprintf(file, "<table>");
	for (int i = 0; i < h; ++i){
		fprintf(file, "<tr>");
		for (int j = 0; j < w; ++j){
			fprintf(file, "<th style='width:10px;height:20px;border:1px solid;background-color:");
			if (*(map + w*i + j) == '#')
				fprintf(file, "green;");
			else
				fprintf(file, "gray;");
			fprintf(file, "'></th>");
		}
		fprintf(file, "</tr>");
	}
	fprintf(file, "</table></body></html>");
	fprintf(file, "<br>");
}
