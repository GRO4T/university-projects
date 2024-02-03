#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define TAB_SIZE 512
#define MAX_POP 3000000000
#define EST_COL 20

int NUM_OF_DATA;

typedef struct Item_t{
	char * key;
	long value;
	struct Item_t * next;
} Item;

typedef struct LinkedList{
	
} LinkedList;

typedef struct HashTable{
	int size;
	int count;	//how full the table is
	int * chains;
	int * collisions;
	Item ** items;
} HashTable;

void get_value(long * val, long min, long max, char * val_name);
int check_buffer(char * buf, int len);
int clear_buffer();
int ask(const char * question);
void open_file(FILE ** file, const char * mode);

static Item * new_item(Item * it, unsigned char * key, long value);
HashTable * new_hash_table();
static Item * delete_item(Item * item);
void delete_hash_table(HashTable * ht);

int hash_function(unsigned char * key);
void insert(HashTable * ht, unsigned char * key, long value);
long get(HashTable * ht, unsigned char * key);
int delete(HashTable * ht, unsigned char * key);

void fill_from_file(HashTable * ht);
void print_col_history(HashTable * ht);

int main(){
	HashTable * ht = new_hash_table();
	if (ask("Czy chcesz wprowadzic dane z pliku .txt?"))
		fill_from_file(ht);

	long c;
	do{
		char key[100] = "";
		long val;

		puts("");
		puts("1. Dodaj element");
		puts("2. Usun element");
		puts("3. Uzyskaj dostep do elementu");
		puts("4. Stopien wypelnienia tablicy");
		puts("5. Histogram kolizji");
		puts("6. Wyjdz");
		get_value(&c, 0, 7, "wybor");
	 	if (c == 1){
			printf("Nazwa: ");
			fgets(key, 98, stdin);

			if (get(ht, key))
				puts("w tablicy jest juz element o takiej nazwie!");
			else{
				insert(ht, key, val);
				get_value(&val, 0, MAX_POP, "liczba ludnosci");
			}
		}
		else if (c == 2){
			printf("Nazwa: ");
			fgets(key, 98, stdin);
			if(delete(ht, key))
				puts("w tablicy nie ma takiego elementu!");
		}
		else if (c == 3){
			printf("Nazwa: ");
			fgets(key, 98, stdin);
			long x = get(ht, key);
			if (x != -1)
				printf("%ld mieszkancow\n", x);
			else
				puts("W tablicy nie ma panstwa o takiej nazwie");
		}
		else if (c == 4){
			double percentage = 100 * (double)ht->count / ht->size;
			printf("Stopien wypelnienia tablicy = %.2f%%\n", percentage);
		}
		else if (c == 5)
			print_col_history(ht);
	} while (c != 6);

	delete_hash_table(ht);
	
	return 0;
}
static Item * new_item(Item * it, unsigned char * key, long value){
	Item * item = malloc(sizeof(Item));

	item->key = strdup(key);
	item->value = value;
	item->next = it;
	
	return item;
}
HashTable * new_hash_table(){
	HashTable * ht = malloc(sizeof(HashTable));

	ht->size = TAB_SIZE;
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(Item*));
	ht->chains = calloc((size_t)ht->size, sizeof(int));
	ht->collisions = calloc(EST_COL, sizeof(int));	
	
	return ht;
}
static Item * delete_item(Item * item){
	Item * next = item->next;
	
	free(item->key);
	free(item);

	return next;
}
void delete_hash_table(HashTable * ht){
	for (int i = 0; i < ht->size; ++i){
		Item * item = ht->items[i];
		while (item != NULL)
			item = delete_item(item);
	}
	free(ht->collisions);
	free(ht->chains);	
	free(ht->items);
	free(ht);
}
int hash_function(unsigned char * key){
	unsigned long hash = 5381;
	int len = strlen(key);

	for (int i = 0; i < len - 1; ++i){
		hash = (hash<<4) + hash + key[i];
	}
	hash = hash % TAB_SIZE;
	return hash;
}
void insert(HashTable * ht, unsigned char * key, long value){
	int i = hash_function(key);
	if (ht->items[i] == NULL){
		++(ht->count);
		ht->chains[i] = 1;
		++(ht->collisions[1]);
	}
	else{
		--(ht->collisions[ht->chains[i]]);
		++(ht->chains[i]);
		++(ht->collisions[ht->chains[i]]);
	}
	ht->items[i] = new_item(ht->items[i], key, value); 
}
long get(HashTable * ht, unsigned char * key){
	int i = hash_function(key);
	
	Item * item = ht->items[i];	
	while (item != NULL && strcmp(item->key, key)){
		item = item->next;
	}
	
	if (item == NULL)
		return -1;
	
	return item->value;
}
int delete(HashTable * ht, unsigned char * key){
	int i = hash_function(key);
		
	if (ht->items[i] == NULL)
		return -1;

	Item * item = ht->items[i];
	Item * prev = NULL;
	while (item != NULL && strcmp(item->key, key)){
		prev = item;
		item = item->next;
	}

	if (item == NULL)
		return -1;
	
	prev->next = delete_item(item);
	--(ht->collisions[ht->chains[i]]);
	--(ht->chains[i]);
	++(ht->collisions[ht->chains[i]]);	
}
void fill_from_file(HashTable * ht){
	FILE * file;

	open_file(&file, "r");
	
	char * line;
	size_t len = 0;
	long val;
	NUM_OF_DATA = 0;
	int c;
	
	while(1){
		getline(&line, &len, file);
		fscanf(file, "%ld", &val);
		c = fgetc(file);
		if ( c != EOF ){
			insert(ht, line, val);
			++NUM_OF_DATA;
		}
		else
			break;
	}
	
	free(line);
}
int ask(const char * question){
	printf("%s T/N ", question);
	int c;

	while(1){
		c = getchar();
		if (c == 't' || c == 'T' || c == 'n' || c == 'N'){
			if (!clear_buffer())
				printf("Mozna odpowiedziec tylko tak lub nie! ");
			else
				break;
		}
		else{	
			printf("Mozna odpowiedziec tylko tak lub nie! ");
			if ( c != '\n' )
				clear_buffer();
		}
	}

	if ( c == 't' || c == 'T' )
		return 1;
	else 
		return 0;
}
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
void open_file(FILE ** file, const char * mode){
	char filename[30] = "";

	if ( (*file = fopen(filename, mode)) == NULL ){
		while (1){	
			printf("nazwa pliku: ");
			scanf("%29s", filename);
			clear_buffer();

			if ( (*file = fopen(filename, mode)) == NULL ){
				puts("Nie mozna otworzyc pliku!");
			}
			else
				break;
		}
	}
}
void get_value(long * val, long min, long max, char * val_name){
	int valid;
	do{
		printf("%s = ", val_name);

		char buffer[10];
		scanf("%10s", buffer);
		clear_buffer();
		valid = check_buffer(buffer, 10);

		if (valid)
			valid = sscanf(buffer, "%10ld", val);

		if (!valid)
			puts("Liczba powinna byc calkowita!");
		else if (*val > max){
			printf("%s powinna byc mniejsza niz %ld\n", val_name, max);
			valid = 0;
		}
		else if (*val <= min){
			printf("%s powinna byc wieksza niz %ld\n", val_name, min);
			valid = 0;
		}
	} while(!valid);
}
void print_col_history(HashTable * ht){
	int x;
	double rows = ht->collisions[1];
	for (int i = 2; i < EST_COL; ++i){
		rows += ht->collisions[i];
		x = ht->collisions[i];
		if (x != 0)
			printf("ilosc kolizji %d. rzedu: %d\n", i - 1, x);
	}
	
	printf("średnia dlugość niepustego wiersza: %.2f\n", NUM_OF_DATA / rows); 
}
