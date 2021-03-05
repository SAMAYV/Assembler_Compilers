// opt represents operation/opcode 
    // opr represents operand 
    // label represents label 
    // addr represents address 
    // LOCCTR represents Location counter
    // fi is file pointer for symbol table file
    // fo is file pointer for opcode table file
    // fp is file pointer for intermediate file
    // ff is file pointer for object code file
    // prog_start stores the starting address of program
    // prog_size stores the program size in hexadecimal
    // intervals is used for finding number of columns in a line
    // line is used for reading a line from input program
    // size is used to calculate how many instructions to write in one text record row of the object code

    // subtract function will subtract 2 hexadecimal numbers
    // remove_trailing_spaces function is used to remove trailing spaces from a string
    // add_trailing_spaces function is used to add trailing spaces till length of string becomes 10
    // Add_Hex function is used for adding 2 hexadecimal numbers
    // decToHexa function is used to convert from decimal to Hexadecimal number
    // find_number_of_columns is used to find in a line whether opcode, operand, labels are present or not
    // hexadecimalToDecimal function is used to convert a value from hexadecimal to decimal