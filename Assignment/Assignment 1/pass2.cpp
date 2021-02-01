#include <bits/stdc++.h>
#include <fstream>
#include "functions.h"

using namespace std;

int main()
{
	fstream fp,fi,fo,ff;
	string counter1, start1, ctr, prog_start, prog_size, line, addr, addr1, label, opt, opr, opt1, opvalue;
	int counter, a, b, start, j, l, k, intervals, i, temp, n;

	fp.open("intermediate_code.txt",ios::in);
    fi.open("symbol_table.txt",ios::in);
    fo.open("opcode_table.txt",ios::in);
    ff.open("object_code.txt",ios::out);

    // opt represents operation 
    // opr represents operand 
    // label represents label 
    // LOCCTR represents Location counter
    // fi is file pointer for intermediate file
    // fo is file pointer for symbol table file
    // fp is file pointer for input program file
	// ff is file pointer for object code file
	// prog_start stores the starting address of program
	// prog_size stores the program size in hexadecimal

	if(!fp || !fi || !fo){
		cout << "File Doesn't Exist!!\n"; 
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
        	for(int k = label.size(); k < 6; k++){
        		ff << " ";
        	}
        	ff << "^00" << ctr;
            while(!fp.eof()){
                getline(fp,line);
                if(line.size() >= 10){
                	addr = line.substr(0,10);
                }
            }
            prog_size = subtract(addr,opr);
            ff << "^00" << prog_size;
            fp.seekg(a,ios::beg);
        }

        // FOR TEXT RECORD-------------------------------------------------------------------------
        
        while(!fp.eof() && opt != "END"){
        	
        	// CALCULATING TEXT RECORD ROW SIZE------------------------------------------------------------

        	a = fp.tellg();
        	int size = 0, prev = 0;
        	fp >> addr;
        	remove_trailing_spaces(addr);

        	fp.seekg(a,ios::beg);
        	start = hexadecimalToDecimal(addr);
        	start1 = addr;

        	do {
        		b = fp.tellg();
        		getline(fp,line);
        		if(line[0] == '.'){
        			continue;
        		}
        		addr = line.substr(0,10);
        		opt = line.substr(20,10);

        		remove_trailing_spaces(addr);
        		remove_trailing_spaces(opt);
        		
        		counter = hexadecimalToDecimal(addr);
        		if(opt != "RESB" && opt != "RESW" && opt != "END"){
        			size = counter-start+3;
        		}
        	} 
        	while(size <= 27 && !fp.eof() && opt != "RESB" && opt != "RESW" && opt != "END");

			//	WRITING TEXT RECORD-------------------------------------------------------------------------

        	if(opt == "END"){
        		size -= 2;
        	}
        	fp.seekg(a,ios::beg);
        	string p = decToHexa(size);
        	if(size < 10){
        		ff << "\nT^00" << start1 << "^0" << p;	
        	}
			else {
				ff << "\nT^00" << start1 << "^" << p;	
			}
			j = 0;
			while(!fp.eof() && j < size){
				l = 0, k = 1;
				fo.seekg(0,ios::beg);
				fi.seekg(0,ios::beg);
				
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
						while(opr[i] != '\''){
							ff << opr[i++];
						}
					}
					else if(opr[0] == 'X' && opr[1] == '\''){
						i = 2;
						ff << "^";
						while(opr[i] != '\''){
							ff << opr[i++];
						}
					}
				}
				else if(opt == "WORD"){
					counter = stoi(opr);
					string val = decToHexa(counter);
					if(counter < 16){
						ff << "^00000" << val;
					}
					else if(counter < 256){
						ff << "^0000" << val;
					}
					else if(counter < 4096){
						ff << "^000" << val;
					}
					else {
						ff << "^00" << val;
					}
				}
				else {
					fo.seekg(0,ios::beg);
					int c = 0;
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
						cout<<opt<<endl;
						continue;
					}
					if(intervals == 1){
						ff << "0000";
					}
					else {
						while(!fi.eof()){
							getline(fi,line);
							addr1 = line.substr(0,10);
							opvalue = line.substr(10,10);
							if(opr == opvalue){
								break;
							}
						}
						counter1 = addr1;
						remove_trailing_spaces(counter1);
						if(l == 0){
							ff << counter1;
						}
						else if(l == 1){
							counter1 = Add_Hex(counter1,"8009"); 
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