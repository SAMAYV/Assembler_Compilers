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
	fstream fp,fi,fo,ff,f;
	string counter1, curr_addr, start_addr, curr_opt, curr_label, obj_code, ctr, prog_start, prog_size, line, addr, addr1, label, opt, opr, opt1, opvalue, p;
	int counter, d, size, a, b, start, j, l, k, intervals, i, temp, n;

	fp.open("intermediate.txt",ios::in);
    ff.open("object.txt",ios::out);
    f.open("debug.txt",ios::out);

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

	while(!fp.eof())
	{

		// WRITING HEADER RECORD-----------------------------------------------------------------------------
		
		getline(fp,line);
		a = fp.tellg();

		if(line.size() == 0 || line[0] == '.'){
			continue;
		}

		addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,20);
        obj_code = line.substr(50,10);

        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);
        remove_trailing_spaces(opt);
        remove_trailing_spaces(opr);
        remove_trailing_spaces(obj_code);

		ctr = addr;
        counter = stoi(addr);
        prog_start = addr;
		
		if(opt == "START" || opt == "CSECT")
        {
        	ff << "H^" << label;
        	for(k = label.size(); k < 6; k++){
        		ff << " ";
        	}
        	ff << "^00" << ctr;
        	curr_opt = "";
            while(!fp.eof() && curr_opt != "CSECT"){
            	d = fp.tellg();
                getline(fp,line);
                
                // means that line is not a comment line
                if(line.size() != 0 && line[0] != '.'){
                	curr_addr = line.substr(0,10);
                	curr_opt = line.substr(20,10);
                	remove_trailing_spaces(curr_opt);
                	remove_trailing_spaces(curr_addr);
                	addr = max_calc(addr,curr_addr);
                }
            }
            // calculating program size
            prog_size = subtract(addr,opr);
            remove_trailing_spaces(prog_size);
            make_size_four(prog_size);
            ff << "^00" << prog_size << "\n";

            // move back the file pointer to second line of intermediate file
            fp.seekg(a,ios::beg);
        }

        // FOR DEFINE RECORD-------------------------------------------------------------------------

        a = fp.tellg();
        getline(fp,line);

        addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,20);
        obj_code = line.substr(50,10);

        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);
        remove_trailing_spaces(opt);
        remove_trailing_spaces(opr);
        remove_trailing_spaces(obj_code);

        map<string,string> mp;

        if(opt == "EXTDEF"){
        	string x = "";
        	for(int z = 0; z < opr.size(); z++){
        		if(opr[z] == ','){
        			mp[x] = "";
        			x = "";
        		}
        		else {
        			x.push_back(opr[z]);
        		}
        	}
        	if(x.size()){
        		mp[x] = 1;
        	}
        	a = fp.tellg();

        	curr_opt = "";
        	while(!fp.eof() && curr_opt != "CSECT"){
            	getline(fp,line);
                
                // means that line is not a comment line
                if(line.size() != 0 && line[0] != '.'){
                	curr_addr = line.substr(0,10);
                	curr_opt = line.substr(20,10);
                	curr_label = line.substr(10,10);
                	remove_trailing_spaces(curr_opt);
                	remove_trailing_spaces(curr_addr);
                	remove_trailing_spaces(curr_label);
                	if(mp.count(curr_label)){
                		mp[curr_label] = curr_addr;
                	}
                }
            }
            ff << "D";
            for(auto it:mp){
            	ff << "^" << it.first << "^00" << it.second;
            }
            ff << "\n";
        }
        fp.seekg(a,ios::beg);

        // FOR REFER RECORD-------------------------------------------------------------------------

        a = fp.tellg();
        getline(fp,line);

        addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,20);
        obj_code = line.substr(50,10);

        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);
        remove_trailing_spaces(opt);
        remove_trailing_spaces(opr);
        remove_trailing_spaces(obj_code);

        map<string,int> ext_ref;

        if(opt == "EXTREF"){
        	ff << "R";
        	string x = "";
        	for(int z = 0; z < opr.size(); z++){
        		if(opr[z] == ','){
        			ff << "^" << x;
        			ext_ref[x] = 1;
        			for(int z = x.size(); z < 6; z++){
        				ff << " ";
        			}
        			x = "";
        		}
        		else {
        			x.push_back(opr[z]);
        		}
        	}
        	if(x.size()){
        		ff << "^" << x;
        		ext_ref[x] = 1;
        		for(int z = x.size(); z < 6; z++){
        			ff << " ";
        		}
        	}
        }
        else {
        	fp.seekg(a,ios::beg);
        }

        // FOR TEXT RECORD---------------------------------------------------------------------------------

        while(!fp.eof())
        {
        	a = fp.tellg();
            f << a << "\n";
        	getline(fp,line);
            
			// means that line is a comment line
			if(line.size() == 0 || line[0] == '.'){
                continue;
            }
            curr_opt = line.substr(20,10);
            remove_trailing_spaces(curr_opt);

            if(curr_opt == "CSECT"){
            	break;
            }

            // CALCULATING TEXT RECORD ROW SIZE------------------------------------------------------------
        	
        	size = 0;
        	start_addr = line.substr(0,10);
        	remove_trailing_spaces(start_addr);

        	fp.seekg(a,ios::beg);
        	start = hexadecimalToDecimal(start_addr);

        	while(!fp.eof()){
        		b = fp.tellg();
        		getline(fp,line);

        		// if line is not a comment line
        		if(line.size() == 0 || line[0] == '.'){
                    continue;
                }
        		
                addr = line.substr(0,10);
	        	opt = line.substr(20,10);
	        	obj_code = line.substr(50,10);

	        	remove_trailing_spaces(addr);
	        	remove_trailing_spaces(opt);
	        	remove_trailing_spaces(obj_code);

                if(opt == "RESB" || opt == "RESW" || opt == "CSECT"){
                    fp.seekg(b,ios::beg);
                    break;
                }
	        	if(size + obj_code.size()/2 < 32){
	        		size += obj_code.size()/2;
	        	}
	        	else {
	        		break;
	        	}
            } 

        	//	WRITING TEXT RECORD-------------------------------------------------------------------------

        	fp.seekg(a,ios::beg);
        	p = decToHexa(size);

        	if(size < 16){
        		ff << "\nT^00" << start_addr << "^0" << p;	
        	}
			else {
				ff << "\nT^00" << start_addr << "^" << p;	
			}

			j = 0;
			while(!fp.eof() && j < size){
                getline(fp,line);

				// means if line is a comment
				if(line.size() == 0 || line[0] == '.'){
	            	continue;
	            }

				addr = line.substr(0,10);
        		label = line.substr(10,10);
        		opt = line.substr(20,10);
        		opr = line.substr(30,20);
        		obj_code = line.substr(50,10);

        		remove_trailing_spaces(addr);
       		 	remove_trailing_spaces(label);
        		remove_trailing_spaces(opt);
        		remove_trailing_spaces(opr);
        		remove_trailing_spaces(obj_code);

        		j += obj_code.size()/2;
        		if(obj_code.size()){
        			ff << "^" << obj_code;
        		}
			}

			// SCANNING CONSECUTIVE RESB OR RESW DEFINING INSTRUCTIONS WHICH DON'T HAVE ANY OBJECT CODE-----
        	while(!fp.eof()){
	            a = fp.tellg();
	            getline(fp,line);
	            if(line.size() == 0 || line[0] == '.'){
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
			    if(opt == "RESB" || opt == "RESW" || opt == "LTORG" || opt == "EQU" || opt == "END"){
                    continue;
                }
                else {
                    fp.seekg(a,ios::beg);
                    break;
                }
            }
        }

        // WRITING END RECORD--------------------------------------------------------------
		ff << "\nE^00" << prog_start << "\n";

        if(!fp.eof()){
			fp.seekg(d,ios::beg);
			ff << "\n";
        }
	}
	fp.close();
	ff.close();
	return 0;
}