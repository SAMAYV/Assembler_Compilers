%{

#include <stdlib.h>
#include <stdio.h>
char* f1, f2, f3;
char arr[50][10];

%}

/* regular expressions */

delim		[ \t\n]
ws			{delim}+
identifier 	[a-zA-Z][a-zA-Z0-9]*
letter 		[a-zA-Z]
number		{digit}+
digit		[0-9]
operation	[-|+|*|/|^|=]

/* rules */

%%

{number}		{
					printf("%d\n", atoi(yytext));
				}
{identifier}	{
					int found = 0;
					for(int i = 1; i < 50; i++){
						if(strcmp(arr[i], yytext) == 0){
							printf("%s\n", yytext);
							found = 1;
						}
					}
					if(!found){
						printf("IDENTIFIER\n");
					}
				}
{delim}			{
				}				

%%

/* auxillary functions */

int yywrap(){
	return 1;
}

void preprocess()
{	
	FILE* fp1 = fopen("tokens.txt", "r");	
	if(fp1 == NULL){
		return;
	} 
	char temp[10];
	for(int i = 0; i < 10; i++){
		temp[i] = '\0';
	}
	int temp_idx = 0, idx = 0, f = 0;
	char c;
	
	while((c = fgetc(fp1)) != EOF) 
	{
		if(c == ' '){ 
			f = 1;
		} 
		else if(c == '\n'){
			strcpy(arr[idx], temp);
			for(int i = 0; i < 10; i++){
				temp[i] = '\0';
			}
			idx = 0;
			temp_idx = 0;
			f = 0;
		}
		else {
			if(f){
				idx = idx*10 + c - '0';
			}
			else {
				temp[temp_idx] = c;
				temp_idx++; 
			}
		} 
	} 
	fclose(fp1);
}

int main(int argc, char* argv[])
{
	preprocess();
	if(argc > 1)
	{
		FILE *fp = fopen(argv[1], "r");
		if(fp){
			yyin = fp;
		}
	}
	yylex();
	return 0;
}
