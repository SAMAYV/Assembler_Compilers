%{

#include <stdlib.h>
#include <stdio.h>

// array to store index as code keys and tokens as values
char arr[50][50];

// used to keep track of line number
int line_no = 1;

// pointer to output file
FILE* out;

// creating node structure for inserting in hash table
struct Node {
	char lexeme[50];
	struct Node* next;
};

// hash table to store the lexemes of int and id
struct Node* table[25];

// some function prototypes
void insert(int index, char* l);
void install_id(char* l);
void install_num(char* l);

%}

/* regular expressions */

delim		[ \t]
line 		\n
whitespace	{delim}+
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
					// storing line number in output.txt
					fprintf(out, "%d\t", line_no);
					
					// NUMBER FOUND
					fprintf(out, "%d\t#%d\n", 23, atoi(yytext));
					
					// inserting lexeme in hash table
					install_num(yytext);
				}
{identifier}	{
					// storing line number in output.txt
					fprintf(out, "%d\t", line_no);
					
					// finding the code for the current token					
					int found = 0;
					for(int i = 1; i < 50; i++)
					{
						if(strcmp(arr[i], yytext) == 0)
						{	
							// TOKEN FOUND
							// storing code for token in output.txt
							fprintf(out, "%d\n", i);
							found = 1;
						}
					}
					if(!found)
					{
						// IDENTIFIER FOUND
						fprintf(out, "%d\t^%s\n", 22, yytext);
						
						// inserting lexeme in hash table
						install_id(yytext);
					}
				}
{delim}			{	
				}
{whitespace}	{
				}
{line}			{
					// increasing line number on finding '\n'
					line_no++;
				}
{operation}		{
					// storing line number in output.txt
					fprintf(out, "%d\t", line_no);
					
					int found = 0;
					
					// finding the code for the current token
					for(int i = 1; i < 50; i++)
					{
						if(strcmp(arr[i], yytext) == 0)
						{
							// TOKEN FOUND
							// storing code for token in output.txt
							fprintf(out, "%d\n", i);
							found = 1;
						}
					}
				}
{assignment}	{
					// storing line number in output.txt
					fprintf(out, "%d\t", line_no);
					
					// TOKEN FOUND
					// storing code for token in output.txt
					fprintf(out, "%d\n", 15);
				}
{end}			{	
					// storing line number in output.txt
					fprintf(out, "%d\t", line_no);
					
					// TOKEN FOUND
					// storing code for token in output.txt
					fprintf(out, "%d\n", 5);	
				}

%%

/* auxillary functions */

int yywrap(){
	return 1;
}

// creating new node and returning a pointer to it
struct Node* newnode()
{
	struct Node* curr = (struct Node*)malloc(sizeof(struct Node));
	curr->next = NULL;
	for(int i = 0; i < 50; i++){
		curr->lexeme[i] = '\0';
	}
	return curr;
}

// inserting in hashtable using code as hashkey
void insert(int index, char* l)
{
	struct Node* curr = table[index];
	if(curr == NULL){
		table[index] = newnode();
		curr = table[index];
	}
	else {
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = newnode();
		curr = curr->next;
	}
	int i = 0;
	while(l[i] != '\0'){
		curr->lexeme[i] = l[i];
		i++;
	}
}

// inserting number lexeme in hash table
void install_num(char* l){
	insert(23, l);
}

// inserting identifier lexeme in hash table
void install_id(char* l){
	insert(22, l);
}

// generating tokens in arr using code as index value through file tokens.txt
void generate_token_table()
{	
	FILE* fp1 = fopen("tokens.txt", "r");	
	if(fp1 == NULL){
		return;
	} 
	char temp[50];
	for(int i = 0; i < 50; i++){
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
			for(int i = 0; i < 50; i++){
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

// doing some preprocess work 
void preprocess()
{
	out = fopen("output.txt", "w");
	for(int i = 0; i < 25; i++){
		table[i] = NULL;
	}
}

// printing the hash table
void print_hash_table()
{
	FILE* hout = fopen("hash_table.txt", "w");
	for(int i = 0; i < 25; i++)
	{
		fprintf(hout, "%d: ", i);
		struct Node* curr = table[i];
		while(curr != NULL){
			fprintf(hout, "%s ", curr->lexeme);
			curr = curr->next;
		}
		fprintf(hout, "\n");
	}
}

int main(int argc, char* argv[])
{
	preprocess();
	generate_token_table();
	if(argc > 1)
	{
		FILE *fp = fopen(argv[1], "r");
		if(fp){
			yyin = fp;
		}
	}
	yylex();
	print_hash_table();
	return 0;
}
