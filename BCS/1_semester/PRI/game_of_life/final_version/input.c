#include"main.h"
#include"input.h"

#define MSG_NN(S) S " was either not a number or the format was wrong. Either way you'll have to type the value by yourself" 
#define MSG_TL(S) S " you provided was too large. You'll have to type it manually"

//czysci zwykly bufor
int clear_buffer(){
	int c;
	int clear = 1;
	do{
		c = getchar();
		if (clear && c != ' ' && c != '\n')
			clear = 0;
	}
	while ( c != '\n' && c != EOF);

	return clear;
}
int check_buffer(char * buf, int len){
	if (buf[0] == '0' && buf[1] != 0)
		return 0;

	int i = 0;
	while (buf[i] != '\0' && i < len){
		if ( (buf[i] != ' ' && buf[i] != '\n') && ( buf[i] < '0' || buf[i] > '9' ) )
			return 0;

		++i;
	}

	return 1;
}
void get_value(int * val, int min, int max, const char * msg, char * val_name){
	puts(msg);

	int valid;
	do{
		printf("%s = ", val_name);

		char buffer[10];
		scanf("%10s", buffer);
		clear_buffer();
		valid = check_buffer(buffer, 10);

		if (valid)
			valid = sscanf(buffer, "%32d", val);

		if (!valid)
			puts("Invalid input!");
		else if (*val > max){
			printf("%s should be less than %d\n", val_name, max);
			valid = 0;
		}
		else if (*val <= min){
			printf("%s should be more than %d\n", val_name, min);
			valid = 0;
		}
	} while(!valid);
}
int ask(const char * question){
	printf("%s Y/N ", question);
	int c;

	while(1){
		c = getchar();
		if (c == 'y' || c == 'Y' || c == 'n' || c == 'N'){
			if (!clear_buffer())
				printf("Answer can only be Y or N ! ");
			else
				break;
		}
		else{	
			printf("Answer can only be Y or N! ");
			if ( c != '\n' )
				clear_buffer();
		}
	}

	if ( c == 'y' || c == 'Y' )
		return 1;
	else 
		return 0;
}
void get_int_from_file(FILE * file, int * val, int len, int min, int max, const char * msg_nn, const char * msg_tl, char * val_name){
	char buff[len];
	for (int i = 0; i < len; ++i)
		buff[i] = 0;

	int c;
	int i = 0;
	int not_number = 0;
	int too_large = 0;

	do{
		c = fgetc(file);

		if( !not_number && ( c < '0' || c > '9' ) ){
			if ( c != ' ' && c != '\n' && c != EOF )
				not_number = 1;
		}
		else if ( i < len ){
			buff[i] = c;
			++i;
		}
		else if ( !not_number && !too_large ){
			too_large = 1;
		}
	} while ( c != ' ' && c != '\n' && c != EOF );

	if (not_number || buff[0] == '0')
		get_value(val, min, max, msg_nn, val_name);
	else if (too_large)
		get_value(val, min, max, msg_tl, val_name);
	else{
		sscanf(buff, "%d", val);
		if (*val > max)
			get_value(val, min, max, msg_tl, val_name);
	}
}

void get_map(FILE * file, char * map){
	int c;
	for (int i = 0; i < h; ++i){
		for (int j = 0; j < w; ++j){
			c = fgetc(file);
			if (c != ' ' && c != EOF && c != '\n')
				*(map + w*i + j) = '#';
			else
				*(map + w*i + j) = ' ';
		}
		fgetc(file);
	}
}
void open_file(FILE ** file, const char * mode){
	char filename[30] = "";

	if ( (*file = fopen(filename, mode)) == NULL ){
		while (1){	
			printf("filename: ");
			scanf("%29s", filename);
			clear_buffer();

			if ( (*file = fopen(filename, mode)) == NULL ){
				puts("Can't open a file");
			}
			else
				break;
		}
	}

	if (!strcmp(mode, "w"))
		check_extension(filename);
}
void get_game_values(char ** map, int argc, char * argv[]){
	parse_args(argc, argv);	

	if ( !ask("Do you want to take input from .txt file?") ){	
		get_value(&w, 0, 100, "Give a width of the plane", "width");
		get_value(&h, 0, 40, "Give a height of the plane", "height");
		get_value(&steps, 0, 10000, "How much turns do you want to simulate?", "steps");
		get_value(&interval, 0, 10000, "Give an interval between the steps", "interval");

		*map = malloc(w * h * sizeof(char));
		for (int i = 0; i < w*h; ++i)
			*(*map + i) = ' ';
	}
	else{
		FILE * file = NULL;

		if (input_file == NULL || (file = fopen(input_file, "r")) == NULL)	
			open_file(&file, "r"); 

		get_int_from_file(file, &w, 3, 0, 200, MSG_NN("Width"), MSG_TL("Width"), "width");
		get_int_from_file(file, &h, 3, 0, 100, MSG_NN("Height"), MSG_TL("Height"), "height");
		get_int_from_file(file, &steps, 5, 0, 99999, MSG_NN("Steps"), MSG_TL("Steps"), "steps");
		get_int_from_file(file, &interval, 5, 0, 99999, MSG_NN("Interval"), MSG_TL("Interval"), "interval");

		*map = malloc(w * h * sizeof(char));	
		get_map(file, *map);	
	}
}	
int check_extension(char * filename){
	int len = strlen(filename);
	char extension[4];
	for (int i = 0; i < len; ++i){
		if (filename[i] == '.'){
			++i;
			if ((len - i) > 4)
				return 0;
			else{
				for (int x = 0; x < len - i + 1; ++x)
					extension[x] = filename[i + x];
			}
			break;
		}	
	}

	if (!strcmp(extension, "txt"))
		filetype = 't';
	else if (!strcmp(extension, "html"))
		filetype = 'h';
	else
		return 0;

	return 1;
}
void parse_args(int argc, char * argv[]){

	for (int i = 0; i < argc; ++i){
		if (!strcmp(argv[i], "i")){
			if (i + 1 < argc){
				++i;
				input_file = malloc(strlen(argv[i]) + 1);
				strcpy(input_file, argv[i]);
			}
		}
		else if (!strcmp(argv[i], "o")){
			if (i + 1 < argc){
				++i;
				if (check_extension(argv[i])){
					output_file = malloc(strlen(argv[i]) + 1);
					strcpy(output_file, argv[i]);
				}
			}
		}
		else if (!strcmp(argv[i], "d")){
			if (i + 1 < argc){
				++i;
				if (strlen(argv[i]) > 5){
					get_value(&delta_steps, 0, 10000, "delta steps - written value too large", "delta steps");
					break;
				}

				sscanf(argv[i], "%d", &delta_steps);
				if ( (argv[i][0] == '0' && strlen(argv[i]) > 1) || !check_buffer(argv[i], strlen(argv[i])) )
					get_value(&delta_steps, 0, 10000, "delta steps - cannot read from command line", "delta steps");
				else if (delta_steps > 10000)
					get_value(&delta_steps, 0, 10000, "delta steps - written value too large", "delta steps");
			}
		}	
	}
}
