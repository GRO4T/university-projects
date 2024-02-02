#include<stdio.h>

int main(){
	FILE * input_file;
	FILE * output_file;
	char in_filename[] = "countries.txt";
	char out_filename[] = "output.txt";
	int c;
	int BRACKET = 0;
	
	input_file = fopen(in_filename, "r");
	output_file = fopen(out_filename, "w");
	
	do{
		c = fgetc(input_file);
		if (BRACKET && c == '['){
			BRACKET = 0;
			c = fgetc(input_file);
			while (c != ']'){
				fprintf(output_file, "%c", c);
				c = fgetc(input_file);
			}
			fprintf(output_file, "\n");
			
			int i = 0;
			do{
				c = fgetc(input_file);
				if (c == '=')
					++i;
			} while (i < 8);
			
			char number[10];
			for (int i = 0; i < 10; ++i)
				number[i] = '\0';
			int k = 0;
			do{
				c = fgetc(input_file);
				if (c >= '0' && c <= '9')
					number[k++] = c; 
			}while (c != '\n');
			int num;
			sscanf(number, "%10d", &num);
			fprintf(output_file, "%d\n", num);
			
		}
		else if ( c == '[' )
			BRACKET = 1;
		else
			BRACKET = 0;	 
	} while(c != EOF);


	return 0;
}
