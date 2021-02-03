/*  
    This Program should be compiled and executed in LINUX g++ environment 
    g++ -o Pass1 180101097_Assign01_Pass1.cpp 
    ./Pass1 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign01_Functions.h"

using namespace std;

int main()
{
	fstream fp,fo,fi;
    int j = 0, intervals, counter, ERROR_FLAG = 0;
    string line, temp, label, opt, opr, LOCCTR;
    
    vector<pair<string,string>> v;

    fp.open("input_program.txt",ios::in);
    fo.open("symbol_table.txt",ios::out);
    fi.open("intermediate_code.txt",ios::out);

    // opt represents operation 
    // opr represents operand 
    // label represents label 
    // LOCCTR represents location counter value
    // ERROR_FLAG will be 1 if any symbol occurred more than one time
    // fi is file pointer for intermediate file
    // fo is file pointer for symbol table file
    // fp is file pointer for input program file
    // intervals is used for finding number of columns in a line
    // line is used for reading a line from input program

    // remove_trailing_spaces function is used to remove trailing spaces from a string
    // add_trailing_spaces function is used to add trailing spaces till length of string becomes 10
    // vector v stores the symbols as keys and their addresses as values pairwise
    // Add_Hex function is used for adding 2 hexadecimal numbers
    // decToHexa function is used to convert from decimal to Hexadecimal number
    // find_number_of_columns is used to find in a line whether opcode, operand, labels are present or not

    if(!fp){
        cout << "File Doesn't Exist" << '\n';
    }
    else {

        // Read First Line
        getline(fp,line);
        label = line.substr(0,10);
        opt = line.substr(10,10);
        opr = line.substr(20,10);
        remove_trailing_spaces(opt);
        remove_trailing_spaces(opr);

        // Checking If Opcode Is START
        if(opt == "START"){

            // Initializing LOCCTR to starting address
            LOCCTR = opr; 
            temp = LOCCTR;

            // adding trailing spaces to write in intermediate file
            add_trailing_spaces(temp);
            fi << temp << line << '\n';
        }
        else {
            // else Initializing LOCCTR to 0
            LOCCTR = "0";                                 
            fp.seekg(0,ios::beg);
        }

        while(!fp.eof()){
        	getline(fp,line);

            // current line is a comment 
        	if(line[0] == '.'){
        		fi << line << '\n';
        		continue;
        	}

            // intervals is used to find whether current line contains opcode, operand, label or not
            intervals = find_number_of_columns(line);
            temp = LOCCTR;
            add_trailing_spaces(temp);
            
            label = line.substr(0,10);
            opt = line.substr(10,10);
            opr = line.substr(20,10);
            remove_trailing_spaces(opt);

            fi << temp << line;
            if(opt != "END"){
                fi << '\n';
            }

            if(intervals == 1){
                if(j > 0){
                    LOCCTR = Add_Hex(LOCCTR,"3");
                }
            }
            else if(intervals == 2){
                if(line[0] == ' '){                                               
                    // Only Opcode And Operand Present
                    if(j > 0){
                        LOCCTR = Add_Hex(LOCCTR,"3");
                    }
                }
                else {
                    // All of them (Label,Opcode And Operand) Are Present
                    if(opt == "END"){
                        break;
                    }
                    fo.seekg(0,ios::beg);

                    // some label occurred >= 2 times if ERROR_FLAG is 1
                    for(auto it:v){
                        if(it.first == label){
                            ERROR_FLAG = 1;
                            break;
                        }
                    }
                    if(ERROR_FLAG){
                        cout<<"ERROR SINCE SOME SYMBOL " << label << "OCCURRED MORE THAN ONE TIME" << '\n';
                    }
                    else {
                    	temp = LOCCTR;
			            add_trailing_spaces(temp);
                        fo.seekg(0,ios::end);
                        v.push_back({label,temp});
                    }
                        
                    // checking the type of operation and incrementing location counter accordingly
                    if(opt == "WORD"){
                        LOCCTR = Add_Hex(LOCCTR,"3");
                    }
                    else if(opt == "RESW"){
                        counter = stoi(opr);
                        temp = decToHexa(3*counter);
                        LOCCTR = Add_Hex(LOCCTR,temp);
                    }
                    else if(opt == "RESB"){
                        counter = stoi(opr);
                        temp = decToHexa(counter);
                        LOCCTR = Add_Hex(LOCCTR,temp);
                    }
                    else if(opt == "BYTE"){
                        if(opr[0] == 'C'){
                            temp = decToHexa(length(opr)-3);
                            LOCCTR = Add_Hex(LOCCTR,temp);
                        }
                        else if(opr[0] == 'X'){
                            temp = decToHexa((length(opr)-3)/2);
                            LOCCTR = Add_Hex(LOCCTR,temp);
                        }
                    }
                    else {
                        LOCCTR = Add_Hex(LOCCTR,"3");
                    }
                }
            }
            j++;
        }
        
        // writing all symbols in the symbol_table.txt file
        for(int i=0;i<v.size()-1;i++){
            fo << v[i].second << v[i].first << '\n';
        }
        if(v.size()){
            fo << v.back().second << v.back().first;
        }
    }
    fp.close();
    fo.close();
    fi.close();
}