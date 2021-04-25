/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _PROGRAM = 258,
    _VAR = 259,
    _BEGIN = 260,
    _END = 261,
    _END_DOT = 262,
    _INTEGER = 263,
    _REAL = 264,
    _FOR = 265,
    _READ = 266,
    _WRITE = 267,
    _TO = 268,
    _DO = 269,
    _SEMI_COLON = 270,
    _COLON = 271,
    _COMMA = 272,
    _ASSIGN = 273,
    _PLUS = 274,
    _MINUS = 275,
    _MULT = 276,
    _DIV = 277,
    _OPEN_PAR = 278,
    _CLOSE_PAR = 279,
    _IDENTIFIER = 280,
    _INT = 281,
    _Real = 282,
    UNEG = 283,
    UPOS = 284
  };
#endif
/* Tokens.  */
#define _PROGRAM 258
#define _VAR 259
#define _BEGIN 260
#define _END 261
#define _END_DOT 262
#define _INTEGER 263
#define _REAL 264
#define _FOR 265
#define _READ 266
#define _WRITE 267
#define _TO 268
#define _DO 269
#define _SEMI_COLON 270
#define _COLON 271
#define _COMMA 272
#define _ASSIGN 273
#define _PLUS 274
#define _MINUS 275
#define _MULT 276
#define _DIV 277
#define _OPEN_PAR 278
#define _CLOSE_PAR 279
#define _IDENTIFIER 280
#define _INT 281
#define _Real 282
#define UNEG 283
#define UPOS 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 41 "180101097.y"
 
	char* p; 

#line 119 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
