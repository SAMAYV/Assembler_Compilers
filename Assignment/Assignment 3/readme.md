- Name: Samay Varshney
- Roll Number: 180101097
- CS348 Assignment03

- tokens.txt contains all the tokens along with their codes.
- input.txt contains the input PASCAL program.

# Instructions for running the code:

	- First run code.l using the following command:
		lex code.l

	- It will generate lex.yy.c file. Now run the following command to generate a.out:
		gcc lex.yy.c

	- Now run a.out by taking the file name input.txt as command line argument using the following command:
		./a.out input.txt

	- It will generate output.txt.

- output.txt contains the tokens for each line in output.txt in the following format:
<line_number> <code_for_the_token_in_the_program> <lexeme>

