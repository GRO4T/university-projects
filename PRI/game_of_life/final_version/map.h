#ifndef MAP_H
#define MAP_H

void map_randomize(char * map, int len);
void map_update(char * map);
void map_draw(char * map);

void save_to_html(FILE * file, char * map);
void save_to_txt(FILE * file, char * map);

void simulate(char * map);

#endif
