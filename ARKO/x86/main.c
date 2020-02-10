#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
//#define ALLEGRO_UNSTABLE
#include <allegro5/allegro.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_image.h>

#include "quadratic_func.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FILE * bitmap_file;
ALLEGRO_BITMAP * bitmap;

int width;
int height;
int bytes_per_row;

char * bmp_ptr; //pointer to the bitmap we will be performing all the drawing on
char * unmod_bmp_ptr; // unmodified bitmap to recover state

bool pressed_keys[ALLEGRO_KEY_MAX];

const char * filename = "uklady/uklad_600_600.bmp";
//const char * filename = "../1024x1024.bmp";

struct Params{
	double a;
	double b;
	double c;
	double step;
	int scale;
} params;

int get4bytes(char *dataptr){
	int ret = 0;
	for(int i = 0; i < 4; ++i){
		ret *= 256;
		ret += *(unsigned char*)dataptr;
		dataptr -= 1;
	}
	return ret;
}

bool redraw = true;

int init();
void draw();
void update_params();

int main(int argc, char *argv[])
{
	params.a = -0.80f;
	params.b = 1.6f;
	params.c = 2.5f;
	params.scale = 4;
	params.step = 4.0f;
	bool running = true;

	//load bitmap
	FILE * bmp = fopen(filename, "rb");
	if (bmp == NULL){
		printf("Error opening bitmap!");
		return 1;
	}
	fseek(bmp, 0, SEEK_END);
	long bmp_size = ftell(bmp);
	fseek(bmp, 0, SEEK_SET);

	char * bmp_ptr = malloc(bmp_size);
	char * unmod_bmp_ptr = malloc(bmp_size);
	int bytes_read = 0;
	bytes_read = fread(bmp_ptr, 1, bmp_size, bmp);
	//strncpy(unmod_bmp_ptr, bmp_ptr, bmp_size);
	memcpy(unmod_bmp_ptr, bmp_ptr, bmp_size);

	if (bytes_read != bmp_size){
		printf("Reading error bmp_size = %ld, bytes read = %d", bmp_size, bytes_read);
		return 1;
	}

	fclose(bmp);

	width = get4bytes(unmod_bmp_ptr+21);
	height = get4bytes(unmod_bmp_ptr+25);
	bytes_per_row = ((int)bmp_size-54) / height;


	//init
	int err_code = init();
	if (err_code != 0)
		return err_code;

	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		switch (event.type) {
			case ALLEGRO_EVENT_KEY_DOWN:
				if (event.keyboard.keycode == ALLEGRO_KEY_2)
					pressed_keys[ALLEGRO_KEY_2] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_1)
					pressed_keys[ALLEGRO_KEY_1] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_4)
					pressed_keys[ALLEGRO_KEY_4] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_3)
					pressed_keys[ALLEGRO_KEY_3] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_6)
					pressed_keys[ALLEGRO_KEY_6] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_5)
					pressed_keys[ALLEGRO_KEY_5] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
					pressed_keys[ALLEGRO_KEY_UP] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
					pressed_keys[ALLEGRO_KEY_DOWN] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
					pressed_keys[ALLEGRO_KEY_LEFT] = true;
				else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					pressed_keys[ALLEGRO_KEY_RIGHT] = true;

				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				if (event.keyboard.keycode == ALLEGRO_KEY_2)
					pressed_keys[ALLEGRO_KEY_2] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_1)
					pressed_keys[ALLEGRO_KEY_1] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_4)
					pressed_keys[ALLEGRO_KEY_4] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_3)
					pressed_keys[ALLEGRO_KEY_3] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_6)
					pressed_keys[ALLEGRO_KEY_6] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_5)
					pressed_keys[ALLEGRO_KEY_5] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
					pressed_keys[ALLEGRO_KEY_UP] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
					pressed_keys[ALLEGRO_KEY_DOWN] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
					pressed_keys[ALLEGRO_KEY_LEFT] = false;
				else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					pressed_keys[ALLEGRO_KEY_RIGHT] = false;

				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				redraw = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			default:
				break;
		}

		// Check if we need to redraw
		if (redraw /*&& al_is_event_queue_empty(event_queue)*/) {
			printf("\n");
			printf("a = %lf\n", params.a);
			printf("b = %lf\n", params.b);
			printf("c = %lf\n", params.c);
			printf("scale = %d\n", params.scale);
			printf("step = %lf\n", params.step);
			update_params();
			// Redraw
			quadratic_func(bmp_ptr + 54, width, height, params.scale, bytes_per_row, params.a, params.b, params.c, params.step);
			bitmap_file = al_open_memfile(bmp_ptr, bmp_size, "r");
			bitmap = al_load_bitmap_f(bitmap_file, ".bmp");
			//strncpy(bmp_ptr, unmod_bmp_ptr, bmp_size);
			memcpy(bmp_ptr, unmod_bmp_ptr, bmp_size);
			if (bitmap == NULL){
				printf("Failed to load bitmap!");
				al_destroy_display(display);
				al_destroy_event_queue(event_queue);
				return 1;
			}

			al_draw_bitmap(bitmap, 0, 0, 0);
			al_flip_display();
            redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

int init(){
	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	// Create the display
	display = al_create_display(width, height);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}

	if (!al_install_keyboard()){
		fprintf(stderr, "Failed to initialize the keyboard!");
	}

	if (!al_init_image_addon()){
		fprintf(stderr, "Failed to initialize image addon!");
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	return 0;
}

void update_params(){
	if (pressed_keys[ALLEGRO_KEY_2] == true)
		params.a += 0.05f;
	if (pressed_keys[ALLEGRO_KEY_4] == true)
		params.b += 0.1f;
	if (pressed_keys[ALLEGRO_KEY_6] == true)
		params.c += 0.1f;
	if (pressed_keys[ALLEGRO_KEY_UP] == true){
		if (params.scale < 120)
			params.scale += 1;
	}
	if (pressed_keys[ALLEGRO_KEY_RIGHT] == true){
		params.step += 1.0f;
		if (params.step > 40.0f)
			params.step = 40.0f;
	}

	if (pressed_keys[ALLEGRO_KEY_1] == true)
		params.a -= 0.05f;
	if (pressed_keys[ALLEGRO_KEY_3] == true)
		params.b -= 0.1f;
	if (pressed_keys[ALLEGRO_KEY_5] == true)
		params.c -= 0.1f;
	if (pressed_keys[ALLEGRO_KEY_DOWN] == true){
		if (params.scale > 1)
			params.scale -= 1;
	}
	if (pressed_keys[ALLEGRO_KEY_LEFT] == true){
		params.step -= 1.0f;
		if (params.step < 1.0f)
			params.step = 1.0f;
	}
}
