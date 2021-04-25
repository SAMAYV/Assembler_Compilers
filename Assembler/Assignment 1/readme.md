- Name: Samay Varshney
- Roll Number: 180101097
- Course: CS348

- 180101097_Assign01_Pass1.cpp contains code of pass1 and 180101097_Assign01_Pass2.cpp contains code of pass2.

- input_program.txt will contain user's assembly code as input. 
- All the lines beginning with '.' in input_program.txt file are treated as comments.

# Each of the LABEL, OPCODE, OPERAND is considered to have 10 character field in the file input_program.txt. 
- So LABEL - 10 chars, OPCODE - 10 chars, OPERAND - 10 chars.

- 180101097_Assign01_Pass1.cpp will use input_program.txt as input file.
- 180101097_Assign01_Pass1.cpp will generate intermediate_code.txt and symbol_table.txt as output files.

- 180101097_Assign01_Pass2.cpp will use intermediate_code.txt, symbol_table.txt and opcode_table.txt as input files.
- 180101097_Assign01_Pass2.cpp will generate object_code.txt as output file.

- After executing both the cpp files, object_code.txt will contain the object code of input assembly code.

# Instructions for running the code:

	1. Both the cpp files should be compiled and executed in LINUX g++ environment. 

	2. First generate intermediate code and symbol table which will be used in generating object code. Use the command:
		$ g++ -o Pass1 180101097_Assign01_Pass1.cpp
		$ ./Pass1

	3. To get object code use the command:
		$ g++ -o Pass2 180101097_Assign01_Pass2.cpp
		$ ./Pass2 																															