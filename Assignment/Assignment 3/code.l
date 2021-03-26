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
end 		END.

/* rules */

%%

{number}		{
					if(!mark_line[line_no]){
						mark_line[line_no] = 1;
						fprintf(out, "%d  ", line_no);	
					}
					else {
						fprintf(out, "   ");
					}
					// NUMBER FOUND
					fprintf(out, "%d %d\n", 23, atoi(yytext));
				}
{identifier}	{
					if(!mark_line[line_no]){
						mark_line[line_no] = 1;
						fprintf(out, "%d  ", line_no);	
					}
					else {
						fprintf(out, "   ");
					}
					
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
						fprintf(out, "%d %s\n", 22, yytext);
					}
				}
{delim}			{	
				}
{line}			{
					line_no++;
				}
{operation}		{
					if(!mark_line[line_no]){
						mark_line[line_no] = 1;
						fprintf(out, "%d  ", line_no);	
					}
					else {
						fprintf(out, "   ");
					}
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
					if(!mark_line[line_no]){
						mark_line[line_no] = 1;
						fprintf(out, "%d  ", line_no);	
					}
					else {
						fprintf(out, "   ");
					}
					// TOKEN FOUND
					fprintf(out, "%d %s\n", 15, yytext);
				}
{end}			{	
					if(!mark_line[line_no]){
						mark_line[line_no] = 1;
						fprintf(out, "%d  ", line_no);	
					}
					else {
						fprintf(out, "   ");
					}
					// TOKEN FOUND
					fprintf(out, "%d %s\n", 5, yytext);	
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
