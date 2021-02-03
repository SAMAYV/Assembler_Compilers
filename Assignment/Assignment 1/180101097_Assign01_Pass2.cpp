/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	g++ -o Pass2 180101097_Assign01_Pass2.cpp 
	./Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign01_Functions.h"

using namespace std;

int main()
{
	fstream fp,fi,fo,ff;
	string counter1, start1, ctr, prog_start, prog_size, line, addr, addr1, label, opt, opr, opt1, opvalue, p;
	int counter, size, a, b, start, j, l, k, intervals, i, temp, n;

	fp.open("intermediate_code.txt",ios::in);
    fi.open("symbol_table.txt",ios::in);
    fo.open("opcode_table.txt",ios::in);
    ff.open("object_code.txt",ios::out);

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

	if(!fp || !fi || !fo){
		cout << "File Doesn't Exist\n"; 
	}
	else {
		// WRITING HEADER RECORD-----------------------------------------------------------------------------

		getline(fp,line);
		a = fp.tellg();

        addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,10);

        remove_trailing_spaces(opr);
        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);
        remove_trailing_spaces(opt);

        ctr = opr;
        counter = stoi(opr);
        prog_start = opr;

        if(opt == "START")
        {
        	ff << "H^" << label;
        	for(k = label.size(); k < 6; k++){
        		ff << " ";
        	}
        	ff << "^00" << ctr;
            while(!fp.eof()){
                getline(fp,line);
                
                // means that line is not a comment line
                if(line[0] != '.'){
                	addr = line.substr(0,10);
                }
            }
            // calculating program size
            prog_size = subtract(addr,opr);
            ff << "^00" << prog_size;

            // move back the file pointer to second line of intermediate file
            fp.seekg(a,ios::beg);
        }

        // FOR TEXT RECORD-------------------------------------------------------------------------
        
        while(!fp.eof() && opt != "END")
        {	
        	// CALCULATING TEXT RECORD ROW SIZE------------------------------------------------------------
        	
        	// current file pointer stored in 'a' so that after calculating size, we can move back to this pointer to read size instructions 
        	a = fp.tellg();
        	size = 0;
        	fp >> addr;
        	remove_trailing_spaces(addr);

        	fp.seekg(a,ios::beg);
        	start = hexadecimalToDecimal(addr);
        	start1 = addr;

        	do {
        		b = fp.tellg();
        		getline(fp,line);

        		// if line is not a comment line
        		if(line[0] != '.'){
        			addr = line.substr(0,10);
	        		opt = line.substr(20,10);

	        		remove_trailing_spaces(addr);
	        		remove_trailing_spaces(opt);
	        		
	        		// counter is current value of address in intermediate file
	        		counter = hexadecimalToDecimal(addr);

	        		if(opt != "RESB" && opt != "RESW" && opt != "END"){
	        			// calculating size using counter
	        			size = counter-start+3;
	        		}
        		}
        	} 
        	while(size <= 27 && !fp.eof() && opt != "RESB" && opt != "RESW" && opt != "END");

			
			//	WRITING TEXT RECORD-------------------------------------------------------------------------

        	// last instruction is of one byte only hence reduce size by 2
        	if(opt == "END"){
        		size -= 2;
        	}

        	fp.seekg(a,ios::beg);
        	p = decToHexa(size);

        	// writing text record in object file
        	// start1 contains hexadecimal value of address from where we have to start writing in current text row 
        	if(size < 10){
        		ff << "\nT^00" << start1 << "^0" << p;	
        	}
			else {
				ff << "\nT^00" << start1 << "^" << p;	
			}

			j = 0;

			// writing till size is achieved
			while(!fp.eof() && j < size){
				l = 0, k = 1;
				fo.seekg(0,ios::beg);
				fi.seekg(0,ios::beg);
				
				getline(fp,line);

				// means if line is a comment
				if(line[0] == '.'){
	            	continue;
	            }

	            addr = line.substr(0,10);
				label = line.substr(10,10);
				opt = line.substr(20,10);
				opr = line.substr(30,10);
				remove_trailing_spaces(addr);
				remove_trailing_spaces(opt);
				temp = hexadecimalToDecimal(addr);

				intervals = find_number_of_columns(line);
				j = temp - start + 3;
				
				if(intervals == 2){
					for(i = 0; i < opr.size()-1; i++){
	                    if(opr[i] == ',' && opr[i+1] == 'X'){
							l = 1;
	                    }
					}
				}
				if(opt == "BYTE"){
					if(opr[0] == 'C' && opr[1] == '\''){
						i = 2;
						ff << "^";

						// to check if it is EOF symbol
						string w;
						while(opr[i] != '\''){
							w.push_back(opr[i++]);
						}
						if(w == "EOF"){
							ff << "454F46";
						}
						else {
							ff << w;
						}
					}
					else if(opr[0] == 'X' && opr[1] == '\''){
						i = 2;
						ff << "^";
						while(opr[i] != '\''){
							ff << opr[i];
							i++;
						}
					}
				}
				else if(opt == "WORD"){
					counter = stoi(opr);
					string val = decToHexa(counter);
					string s = "^00000";
					if(counter < 16){
						
					}
					else if(counter < 256){
						s.pop_back();
					}
					else if(counter < 4096){
						s.pop_back();
						s.pop_back();
					}
					else {
						s.pop_back();
						s.pop_back();
						s.pop_back();
					}
					ff << s << val;
				}
				else {

					// move to beginning of opcode file
					fo.seekg(0,ios::beg);

					// search for opcode in file
					while(!fo.eof()){
						fo >> opt1 >> opvalue;
						if(opt == opt1){
							k = 0;
							break;
						}
					}
					if(k == 0){
						ff << "^" << opvalue;
					}
					else {
						cout<<"SOME OPCODE NOT FOUND: "<<opt<<"...INVALID PROGRAM CODE\n";
					}

					if(intervals == 1){
						ff << "0000";
					}
					else {
						// if ,X type of operands are present then need to remove ,X while checking in symbol table file
						for(int z = 0; z < 10; z++){
							if(opr[z] == ','){
								opr = opr.substr(0,z);
								add_trailing_spaces(opr);
							}
						}
						k = 1;
						// reading symbol table file and checking for operand
						while(!fi.eof()){
							getline(fi,line);
							addr1 = line.substr(0,10);
							opvalue = line.substr(10,10);
							if(opr == opvalue){
								k = 0;
								break;
							}
						}
						if(k){
							cout<<"SOME SYMBOL/OPERAND NOT FOUND: "<<opr<<"...INVALID PROGRAM CODE\n";		
						}
						counter1 = addr1;
						remove_trailing_spaces(counter1);
						if(l == 0){
							ff << counter1;
						}
						else if(l == 1){
							// used for ,X type of operands
							counter1 = Add_Hex(counter1,"8000"); 
							ff << counter1;
						}
					}
				}
			}

			// SCANNING CONSECUTIVE RESB OR RESW DEFINING INSTRUCTIONS WHICH DON'T HAVE ANY OBJECT CODE-----
        	do {
	            a = fp.tellg();
	            getline(fp,line);
	            if(line[0] == '.'){
	            	continue;
	            }
	            addr = line.substr(0,10);
				label = line.substr(10,10);
				opt = line.substr(20,10);
				opr = line.substr(30,10);
				remove_trailing_spaces(addr);
				remove_trailing_spaces(opt);
				remove_trailing_spaces(label);
				remove_trailing_spaces(opr);
			}
			while(opt == "RESB" || opt == "RESW");
			fp.seekg(a,ios::beg);
        }

		// WRITING END RECORD--------------------------------------------------------------
		ff << "\nE^00" << prog_start;
	}
	fp.close();
	fi.close();
	fo.close();
	ff.close();
	return 0;
}