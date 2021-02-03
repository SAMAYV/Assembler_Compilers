- pass1.cpp contains code of pass1 and pass2.cpp contains code of pass2.
- pass1.cpp will use input_program.txt as input file.

- input_program.txt will contain user's assembly code as input. 

# Each of the LABEL, OPCODE, OPERAND is considered to have 10 character field in the file input_program.txt. 

- pass1.cpp will generate intermediate_code.txt and symbol_table.txt as output files.

- pass2.cpp will use intermediate_code.txt, symbol_table.txt and opcode_table.txt as input files.
- pass2.cpp will generate object_code.txt as output file.
- object_code.txt after executing both the cpp files, object_code.txt will contain the desired object code.

# Instructions for running the code:

	1. Both the cpp files should be compiled and executed in LINUX g++ environment 

	2. First generate intermediate code and symbol table which will be used in generating object code. Use the command:
		$ g++ -o Pass1 180101097_Assign01_Pass1.cpp
		$ ./Pass1

	3. To get object code use the command:
		$ g++ -o Pass2 180101097_Assign01_Pass2.cpp
		$ ./Pass2 																															