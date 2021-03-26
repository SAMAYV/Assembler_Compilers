%{

#include <stdlib.h>
#include <stdio.h>
char arr[50][10];
int line_no = 1;
int mark_line[50];
FILE* out;

%}

/* regular expressions */

delim		[ \t]
line 		\n
ws			{delim}+
identifier 	[a-zA-Z][a-zA-Z0-9]*
letter 		[a-zA-Z]
number		{digit}+
digit		[0-9]
operation	[-|+|*|/|^|=|;|:|,|(|)]
assignment	:=
end 		END\.

/* rules */

%%

{number}		{
					fprintf(out, "%d\t", line_no);
					// NUMBER FOUND
					fprintf(out, "%d\t#%d\n", 23, atoi(yytext));
				}
{identifier}	{
					fprintf(out, "%d\t", line_no);
					
					int found = 0;
					for(int i = 1; i < 50; i++){
						if(strcmp(arr[i], yytext) == 0){
							// TOKEN FOUND
							fprintf(out, "%d\n", i);
							found = 1;
						}
					}
					if(!found){
						// IDENTIFIER FOUND
						fprintf(out, "%d\t^%s\n", 22, yytext);
					}
				}
{delim}			{	
				}
{line}			{
					line_no++;
				}
{operation}		{
					fprintf(out, "%d\t", line_no);
					
					int found = 0;
					for(int i = 1; i < 50; i++){
						if(strcmp(arr[i], yytext) == 0){
							// TOKEN FOUND
							fprintf(out, "%d\n", i);
							found = 1;
						}
					}
				}
{assignment}	{
					fprintf(out, "%d\t", line_no);
					
					// TOKEN FOUND
					fprintf(out, "%d\n", 15);
				}
{end}			{	
					fprintf(out, "%d\t", line_no);
					
					// TOKEN FOUND
					fprintf(out, "%d\n", 5);	
				}

%%

/* auxillary functions */

int yywrap(){
	return 1;
}

void temp(){
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
	for(int i = 1; i < 50; i++){
		mark_line[i] = 0;
	}
	out = fopen("output.txt", "w");
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
