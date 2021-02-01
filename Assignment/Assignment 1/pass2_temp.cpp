#include <bits/stdc++.h>
#include <fstream>
#include "functions.h"

using namespace std;

int main()
{
	fstream fp,fi,fo,ff;
	string counter1, start1, ctr, Pstart, Psize, ch, addr, addr1, label, opt, opr, opt1, opvalue;
	int counter, a, b, start, j, l, k, sp, i, temp, n, m = 0;

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

	if(!fp || !fi || !fo){
		cout << "File Doesn't Exist!!\n"; 
	}
	else {
		// WRITING HEADER RECORD-----------------------------------------------------------------------------

		getline(fp,ch);
		a = fp.tellg();
        addr = ch.substr(0,10);
        label = ch.substr(10,10);
        opt = ch.substr(20,10);
        opr = ch.substr(30,10);

        remove_trailing_spaces(opr);
        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);

        ctr = opr;
        counter = stoi(opr);
        Pstart = opr;

        if(opt == "START     ")
        {
        	ff << "H^" << label;
        	for(int k = label.size();k < 6; k++){
        		ff << " ";
        	}
        	ff << "^00" << ctr;
            while(!fp.eof()){
                getline(fp,ch);
                if(ch.size() >= 10){
                	addr = ch.substr(0,10);
                }
            }
            Psize = subtract(addr,opr);
            ff << "^00" << Psize;
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
        		getline(fp,ch);
        		if(ch[0] == '.'){
        			continue;
        		}
        		addr = ch.substr(0,10);
        		opt = ch.substr(20,10);

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
				l = 0, k = 1, sp = 0;
				fo.seekg(0,ios::beg);
				fi.seekg(0,ios::beg);
				
				getline(fp,ch);
				if(ch[0] == '.'){
	            	continue;
	            }
	            addr = ch.substr(0,10);
				label = ch.substr(10,10);
				opt = ch.substr(20,10);
				opr = ch.substr(30,10);
				remove_trailing_spaces(addr);
				remove_trailing_spaces(opt);
				temp = hexadecimalToDecimal(addr);

				for(i = 0;i < ch.size()-1;i++){
					if(ch[i] == ' ' && ch[i+1] != ' '){
						sp++;
					}
				}
				j = temp - start + 3;
				
				if(sp == 2){
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
					if(sp == 1){
						ff << "0000";
					}
					else {
						while(!fi.eof()){
							getline(fi,ch);
							addr1 = ch.substr(0,10);
							opvalue = ch.substr(10,10);
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
	            getline(fp,ch);
	            if(ch[0] == '.'){
	            	continue;
	            }
	            addr = ch.substr(0,10);
				label = ch.substr(10,10);
				opt = ch.substr(20,10);
				opr = ch.substr(30,10);
				remove_trailing_spaces(addr);
				remove_trailing_spaces(opt);
			}
			while(opt == "RESB" || opt == "RESW");
			fp.seekg(a,ios::beg);
        }

		// WRITING END RECORD--------------------------------------------------------------
		ff << "\nE^00" << Pstart;
	}
	fp.close();
	fi.close();
	fo.close();
	ff.close();
	return 0;
}