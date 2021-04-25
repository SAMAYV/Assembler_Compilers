%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "y.tab.h"
int line = 1;

%}

%%

";"			{	return _SEMI_COLON;		}
":"			{	return _COLON;			}
","			{	return _COMMA;			}
":="		{	return _ASSIGN;			}
"+"			{	return _PLUS;			}
"-"			{	return _MINUS;			}
"*"			{	return _MULT;			}
"DIV"		{	return _DIV;			}
"("			{	return _OPEN_PAR;		}
")"			{	return _CLOSE_PAR;		}

\n 			{	line++;					}
"PROGRAM"	{	return _PROGRAM;		}
"VAR"		{	return _VAR;			}	
"BEGIN"		{	return _BEGIN;			}
"END"		{	return _END;			}
"END."		{	return _END_DOT;		}	
"INTEGER"	{	return _INTEGER;		}
"REAL"		{	return _REAL;			}
"FOR"		{	return _FOR;			}
"READ"		{	return _READ;			}
"WRITE"		{	return _WRITE;			}
"TO"		{	return _TO;				}
"DO"		{	return _DO;				}

[0-9]+					{	return _INT;	}

[a-zA-Z]([a-zA-Z0-9_])*	{	
							yylval.p = strdup(yytext); 
							return _IDENTIFIER;	
						}	
[ \t\n]					;

%%

