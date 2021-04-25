- Name: Samay Varshney
- Roll Number: 180101097
- CS348 Assignment04

- The code should run in linux environment.

- 180101097.l contains the lexical code.
- 180101097.y contains the yacc code.

- Total 6 test files are present to cover different error cases. 
1 test file is completely error free named correct_1.pas
3 test files are having syntax errors named syntax_1.pas, syntax_2.pas and syntax_3.pas
2 test files are having semantics errors named semantics_1.pas and semantics_2.pas

# Instructions for running the code:

	- First run 180101097.y using the following command. It will generate y.tab.c and y.tab.h.
		yacc -yd 180101097.y

	- Then run the following command. It will generate lex.yy.c.
		lex 180101097.l

	- Now generate a.out using the following command:
		gcc -o a.out y.tab.c lex.yy.c -lfl -lm

	- Finally for each test case file, run the following command:
		./a.out < file_name

	Here file_name is the name of one of the test case files.

- Output: It will print the errors (if any) on the terminal. If no errors are found, it will print parsing is successful.