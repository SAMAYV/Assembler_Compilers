%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int success = 1;
int count = 0;
int mod = 53;

extern int line;
extern char* yytext;

struct Node
{
	char* lexeme;
	int token_code;
	int type_id;
	struct Node* next;
};

struct Node* table[53];
int type[100];

void set(int, int s);
int has(int, char*);

int hash_func(char*);
int search(char*, int);
void insert(int, char*);
void install_num(char*, int, int);
void install_id(char*, int, int);
struct Node* create_node(char*, int);
void setting_type(int);

int yyerror(char const *s);
int yylex(void);

%}

%union { 
	char* p; 
} 

%token _PROGRAM _VAR _BEGIN _END _END_DOT _INTEGER _REAL _FOR _READ _WRITE _TO _DO 
%token _SEMI_COLON _COLON _COMMA _ASSIGN _PLUS _MINUS _MULT	_DIV _OPEN_PAR _CLOSE_PAR	
%token <p> _IDENTIFIER		
%token <integer> _INT
%token <float> _Real

%nonassoc '='
%left '+' '-'
%left '*' '/'
%left UNEG UPOS

%start prog

%%

prog 			:  _PROGRAM prog_name _VAR dec_list _BEGIN stmt_list _END_DOT 
				;

prog_name 		: _IDENTIFIER	

dec_list 		: dec 
 				| dec_list _SEMI_COLON dec 
 				; 

dec 			: dec_id_list type {
					yyerrok;
					yyerror("");
					printf("Error: colon not present at line number %d\n", line);
					printf("Suggestion: Add colon at line number %d after the variables\n\n", line);
				}
				| dec_id_list _COLON {
					yyerrok;
					yyerror("");
					printf("Error: Type name is not present at line number %d\n", line - 1);
					printf("Suggestion: Add type name at line number %d after the colon\n\n", line - 1);
				}
				| dec_id_list _COLON type
				| error dec {
					yyerrok;
					yyerror("");
					printf("Error: semicolon is missing at line number %d\n", line - 1);
					printf("Suggestion: Add semicolon at the end of statement at line number %d\n\n", line - 1);
				}
				;

type 			: _INTEGER {
					set(1, _INTEGER);
				}
				| _REAL {
					set(1, _REAL);
				}
				;

dec_id_list 	: _IDENTIFIER {
					install_id($1, _IDENTIFIER, 1);
				} 
				| dec_id_list _COMMA _IDENTIFIER {
					install_id($3, _IDENTIFIER, 1);
				}
				;

id_list 		: _IDENTIFIER  
				| id_list _COMMA _IDENTIFIER
				;

stmt_list 		: stmt
		  		| stmt_list _SEMI_COLON stmt 
		  		;

stmt 			: assign 
				| read 
				| write
				| for 
				| error stmt {	
					yyerrok;
					yyerror("");
					printf("Error: semicolon is missing at line number %d\n", line - 1);
					printf("Suggestion: Add semicolon at the end of statement at line number %d\n\n", line - 1);
				}
				;

assign 			: _IDENTIFIER _ASSIGN exp {
					if(!search($1, 1)){
						yyerrok;
						yyerror("");
						printf("Error: variable is undeclared at line number %d\n", line);
						printf("Suggestion: Use declared variables or declare variable at line number %d\n\n", line);
					}
					else {
						if(type[count] == -1){
							yyerrok;
							yyerror("");
							printf("Error: Assignment type mismatch at line number %d\n", line);
							printf("Suggestion: Use variables of same type in statement at line number %d\n\n", line);
						}
						else if(has(-1, $1) != type[count]){
							yyerrok;
							yyerror("");
							printf("Error: Assignment type mismatch at line number %d\n", line);
							printf("Suggestion: Use variables of same type in statement at line number %d\n\n", line);
						}
					}
				}
	   			;

exp 			: term 
				| exp _PLUS term 
				| exp _MINUS term
				;

term 			: factor
				| term _MULT factor
				| term _DIV factor 
				; 

factor 			: _IDENTIFIER {
					int id = has(-1, $1);
					setting_type(id);
				}
			    | _INT {
			    	int id = _INTEGER;
			    	setting_type(id);
				}
			    | _Real {
			    	int id = _REAL;
				    setting_type(id);
			    }
			    | _OPEN_PAR exp _CLOSE_PAR 
			    ;	  

read 			: _READ _OPEN_PAR id_list _CLOSE_PAR 
	 			;

write 			: _WRITE _OPEN_PAR id_list _CLOSE_PAR 
	  			;	

for 			: _FOR index_exp _DO body
				;

index_exp 		: _IDENTIFIER _ASSIGN exp _TO exp 
		  		;

body 			: stmt 
	 			| _BEGIN stmt_list _END
	 			;

%%

void setting_type(int id)
{
	if(type[count] == 0){
		type[count] = id;
	}
	else if(type[count] != -1){
		if(type[count] != id){
			type[count] = -1;
		}
	}
}

struct Node* create_node(char* lexeme, int code)
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
	temp->token_code = code;
	temp->lexeme = lexeme;
	temp->type_id = 0;
	temp->next = NULL;
	return temp;
}


int hash_func(char* s)
{
	int sum = 0, i = 0;
	while(s[i] != '\0')
	{
		if(s[i] >= '0' && s[i] <= '9'){
			sum += 2*(s[i] - '0');
		}
		else {
			sum += 3*(s[i] - 'A');
		}
		sum %= mod;
		i++;
	}
	return sum;
}

void insert(int token_code, char* token)
{	
	int index = hash_func(token);
	struct Node* curr = table[index];

	if(curr == NULL){
		table[index] = create_node(token, token_code);
	}
	else {
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = create_node(token, token_code);
	}
}

int search(char* token, int f)
{
	f = 0;	
    int index = hash_func(token);
    struct Node* it = table[index];
    while(it)
    {
        if(strcmp(token, it->lexeme) == 0){
        	f = 1;
        	break;
        }
        it = it->next;
    }
    return f;
}

int has(int type_id, char* s)
{	
	int index = hash_func(s);
    struct Node* it = table[index];
    while(it)
    {
        if(!strcmp(s, it->lexeme)){
        	type_id = it->type_id;
        	break;
        }
        it = it->next;
    }
    return type_id;
}

void set(int incr, int tok)
{	
	struct Node* it;
	for(int i = 0; i < mod; i+=incr)
	{
		it = table[i];
		while(it){
			if(it->type_id == 0){ 
				it->type_id = tok;
			}
			it = it->next;
		}
	}
}

void install_id(char* token, int code, int flag)
{	
	if(!flag){
		return;
	}
	if(search(token, 1) == 0){
    	insert(code, token);
	}
}

void install_num(char* token, int code, int flag)
{	
	if(!flag){
		return;
	}
	if(search(token, 1) == 0){
    	insert(code, token);
	}
}

int main(int argc, char* argv[]) 
{
	yyparse();
	if(success){
		printf("Parsing completed & is successful\n");
	} 
	else {
		printf("Parsing is not successful\n");
	}
	return 0;
}

int yyerror(char const *s) 
{
	success = 0;
	return 0;
}
