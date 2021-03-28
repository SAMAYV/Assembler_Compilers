- Name: Samay Varshney
- Roll Number: 180101097
- CS348 Assignment03

- tokens.txt contains all the tokens along with their code values.
- input.txt contains the input PASCAL program.

# Instructions for running the code:

	- First run code.l using the following command:
		lex code.l

	- It will generate lex.yy.c file. Now run the following command to generate a.out:
		gcc lex.yy.c

	- Now run a.out by taking the file name input.txt as command line argument using the following command:
		./a.out input.txt

	- It will generate output.txt and hash_table.txt.

- output.txt contains in each line the following values in output.txt: <line_number> <token_type> <token_specifier>
- hash_table.txt contains the lexemes of integer and identifiers using the hash function as code values.
