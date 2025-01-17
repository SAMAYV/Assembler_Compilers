/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	<number> = 1 or <number> = 2

	g++ -o SICXE_Pass1 180101097_Assign02_SICXE_Pass1.cpp 
	./SICXE_Pass1 <number>

*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

// opt represents operation/opcode 
// opr represents operand 
// label represents label 
// addr represents address 
// LOCCTR represents Location counter
// fo is file pointer for opcode table file
// fi is file pointer for intermediate code file
// fs is file pointer for symbol table file
// line is used for reading a line from input program

int main(int argc, char* argv[])
{
	fstream fp,fo,fi,fs;
    int a, i, j = 0, ERROR_FLAG = 0, line_no = 1, ctrl_section = 0;
    string line, temp, label, opt, opr, opt1, addr, obj_code, LOCCTR, p, code, val, z, add, sub, chk, str1, str2, str3;

    // finding the name of input assembly program file
    if(argc > 1){
    	string file = "assembly_program" + string(argv[1]) + ".txt";
    	fp.open(file.c_str(),ios::in);
    }
    else {
    	fp.open("assembly_program1.txt",ios::in);	
    }

    fo.open("opcode_table.txt",ios::in);
    fi.open("intermediate.txt",ios::out);
    fs.open("symbol_table.txt",ios::out);

    map<int,string> addr_map;
    map<string,bool> occur;
    vector<pair<string,int>> literal_table;
    map<int,int> obj_size;
    map<string,vector<pair<int,string>>> label_to_address; 
    map<string,string> symbol_label_addr, opcode_table;
    map<string,string> absolute_relative;
    map<char,string> reg_map = {{'A', "0"}, {'X',"1"}, {'L',"2"}, {'B',"3"}, {'S',"4"}, {'T',"5"}, {'F',"6"}};

    while(!fp.eof())
    {
    	LOCCTR = "0000";
    	occur.clear();
    	symbol_label_addr.clear();
    	absolute_relative.clear();
    	getline(fp,line);
    	if(!line.size() || line[0] == '.'){
	    	line_no++;
	    	continue;
	    }
    	label = line.substr(0,10);
    	remove_trailing_spaces(label);
	    addr_map[line_no] = LOCCTR;
	    label_to_address[label].push_back({ctrl_section,LOCCTR});
	    symbol_label_addr[label] = LOCCTR;
	    absolute_relative[label] = "R";
	    line_no++;

	    while(!fp.eof())
	    {
	    	a = fp.tellg();
	    	getline(fp,line);
	    	if(!line.size() || line[0] == '.'){
	    		line_no++;
	    		continue;
	    	}
	    	label = line.substr(0,10);
	    	opt = line.substr(10,10);
	    	opr = line.substr(20,30);
	    	remove_trailing_spaces(label);
	    	remove_trailing_spaces(opt);
	    	remove_trailing_spaces(opr);

	    	if(opt == "EXTREF" || opt == "EXTDEF" || opt == "END"){
	    		if(opt == "END"){
	    			symbol_label_addr[opt] = opr;
	    			absolute_relative[opt] = "R";
	    		} 
	    		line_no++;
	    		continue;
	    	}
	    	// PROCESSING ADDRESS FOR CONSTANT LITERALS
	    	else if(opt == "LTORG"){
	    		line_no++;
	    		for(auto it:literal_table){
	    			addr_map[line_no] = LOCCTR;
	    			obj_size[line_no] = it.second;
	    			label_to_address[it.first].push_back({ctrl_section,LOCCTR});
	    			symbol_label_addr[it.first] = LOCCTR;
	    			absolute_relative[it.first] = "R";
	    			LOCCTR = Add_Hex(LOCCTR,decToHexa(it.second));
	    			line_no++;
	    		}
	    		literal_table.clear();
	    		continue;
	    	}
	    	if(opt == "CSECT"){
	    		fp.seekg(a,ios::beg);
	    		break;
	    	}
	    	addr_map[line_no] = LOCCTR;
	    	if(label.size()){
	    		symbol_label_addr[label] = LOCCTR;
	    		absolute_relative[label] = "R";
	    		label_to_address[label].push_back({ctrl_section,LOCCTR});
	    	}
	    	if(opt[0] == '+'){
	    		LOCCTR = Add_Hex(LOCCTR,"4");
	    		obj_size[line_no] = 4;
	    	}
	    	else if(opt == "CLEAR" || opt == "COMPR" || opt == "TIXR" || opt == "ADDR" || opt == "DIVR" || opt == "MULR" || opt == "SUBR"){
	    		LOCCTR = Add_Hex(LOCCTR,"2");
	    		obj_size[line_no] = 2;
	    	}
	    	else if(opt == "BYTE"){
	   		 	LOCCTR = Add_Hex(LOCCTR,"1");
	   		 	obj_size[line_no] = 1;
	   		}
	    	else if(opt == "RESW"){
	   			int k = 3*stoi(opr);
	    		LOCCTR = Add_Hex(LOCCTR,decToHexa(k));
	    	}
	    	else if(opt == "RESB"){
	    		int k = stoi(opr);
	    		LOCCTR = Add_Hex(LOCCTR,decToHexa(k));
	    	}
	    	else if(opt == "EQU"){
	    		string loc = "0";
	    		temp = "";
	    		int f = 1;
	    		if(opr != "*")
	    		{
	    			int ctr = 0;
	    			for(i = 0; i < opr.size()+1; i++)
	    			{
	    				if(i == opr.size() || opr[i] == '-' || opr[i] == '+'){
	    					if(f){
	    						if(symbol_label_addr.count(temp)){
	    							loc = Add_Hex(loc,symbol_label_addr[temp]);
	    							ctr++;
	    						}
	    						else {
	    							loc = Add_Hex(loc,temp);
	    						}
	    					} 
	    					else {
	    						if(symbol_label_addr.count(temp)){
	    							loc = subtract(loc,symbol_label_addr[temp]);	
	    							ctr--;
	    						}
	    						else {
	    							loc = subtract(loc,temp);
	    						}
	    					}
	    					temp = "";
	    					if(opr[i] == '-'){
	    						f = 0;
	    					}
	    					else {
	   							f = 1;
	   						}
	   					}
	    				else {
	    					temp.push_back(opr[i]);
	    				}
	    			}
	    			symbol_label_addr[label] = loc;
	    			if(abs(ctr) > 0){
	    				absolute_relative[label] = "R";
	    			}
	    			else {
	    				absolute_relative[label] = "A";
	    			}
	    			label_to_address[label].push_back({ctrl_section,LOCCTR});
	    			addr_map[line_no] = loc;
	    		}
	    	}
	    	else {
	   			LOCCTR = Add_Hex(LOCCTR,"3");
	   			obj_size[line_no] = 3;
	   		}

	   		// ADDING LITERAL CONSTANTS IN LTORG TABLE
	   		if(opr[0] == '='){
	   			p = opr.substr(3,opr.size()-4);
	   			if(!occur.count(p)){
	   				if(p == "EOF"){
	   					literal_table.push_back({opr,3});
	   				}
	   				else {
	   					literal_table.push_back({opr,p.size()/2});
	   				}
	   				occur[opr] = 1;
	   			}
	   		}
	   		line_no++;
	   	}

	   	// PROCESSING OF LITERAL TABLE AFTER THE END OF EACH CONTROL SECTION
	   	for(auto it:literal_table){
	    	addr_map[line_no] = LOCCTR;
	    	obj_size[line_no] = it.second;
	    	label_to_address[it.first].push_back({ctrl_section,LOCCTR});
	    	symbol_label_addr[it.first] = LOCCTR;
	    	absolute_relative[it.first] = "R";
	    	LOCCTR = Add_Hex(LOCCTR,decToHexa(it.second));
	    	line_no++;
	    }
	    literal_table.clear();

	    // WRITING SYMBOLS IN FILE
	    for(auto it:symbol_label_addr){
	    	str1 = it.first;
	    	str2 = it.second;
	    	str3 = absolute_relative[it.first];
	    	add_trailing_spaces(str1);
	    	add_trailing_spaces(str2);
	    	add_trailing_spaces(str3);
	    	fs << str1 << str2 << str3 << "\n";
 	    }
 	    fs << "\n";

 	    // INCREMENTING CONTROL SECTION NUMBER
	    ctrl_section++;
    }

    // BUILDING OPCODE TABLE
    while(!fo.eof()){
    	fo >> code >> val;
    	opcode_table[code] = val;
    }

    fp.seekg(0,ios::beg);
    line_no = 1;
    ctrl_section = 0;
    literal_table.clear();

    while(!fp.eof())
    {
    	occur.clear();
    	getline(fp,line);
	    if(!line.size() || line[0] == '.'){
	    	line_no++;
	    	continue;
	    }
	    z = addr_map[line_no];
	    add_trailing_spaces(z);
    	fi << z << line << "          \n";
    	line_no++;

    	while(!fp.eof())
    	{
    		a = fp.tellg();
    		getline(fp,line);
	    	if(!line.size() || line[0] == '.'){
	    		line_no++;
	    		continue;
	    	}
	    	opt = line.substr(10,10);
	    	remove_trailing_spaces(opt);
	    	
			if(opt == "CSECT"){
	    		fp.seekg(a,ios::beg);
	    		break;
	    	}
	    	if(opt == "EXTREF" || opt == "EXTDEF" || opt == "END"){
	    		fi << "          " << line << "          ";
	    		if(!fp.eof() || literal_table.size()){
	    			fi << "\n";
	    		}
	    		line_no++;
	    		continue;
	    	}

	    	z = "";
	    	if(addr_map.count(line_no)){
	    		z = addr_map[line_no];
	    	}
	    	add_trailing_spaces(z);
	    	fi << z << line;
	    	
	    	z = "";
	    	if(!obj_size.count(line_no)){
	    		add_trailing_spaces(z);
	    		fi << z << "\n";
	    	}

	    	// WRITING LITERALS IN INTERMEDIATE FILE USING LITERAL TABLE
		    if(opt == "LTORG"){
		    	line_no++;
		    	for(auto it:literal_table){
		    		addr = addr_map[line_no];
		    		label = "*";
		    		opt1 = it.first;
		    		opr = "";
		    		obj_code = opt1.substr(3,opt1.size()-4);
		    		if(obj_code == "EOF"){
		    			obj_code = "454F46";
		    		}
		    		add_trailing_spaces(addr);
		    		add_trailing_spaces(label);
		    		add_trailing_spaces(opt1);
		    		add_trailing_spaces(obj_code);
		    		add_trailing_spaces1(opr);
		    		fi << addr << label << opt1 << opr << obj_code << "\n";
		    		line_no++;
		    	}
		    	literal_table.clear();
		    	continue;
		    }
		    if(!obj_size.count(line_no)){
		    	line_no++;
		    	continue;
		    }
	    	
		   	// CALCULATING OBJECT CODE FOR EACH INSTRUCTION

	    	label = line.substr(0,10);
		    opt = line.substr(10,10);
		    opr = line.substr(20,30);
		    remove_trailing_spaces(label);
		    remove_trailing_spaces(opt);
		    remove_trailing_spaces(opr);
	    	
	    	z = "00";
	    	if(obj_size[line_no] == 1){
	    		if(opr.size()){
	    			z = opr.substr(2,opr.size()-3);	
	    		}
	    	}
	    	else if(obj_size[line_no] == 2){
	    		z = opcode_table[opt];
	    		if(opr.size() > 2){
	    			z = z + reg_map[opr[0]] + reg_map[opr[2]];
	    		}
	    		else if(opr.size()){
	    			z = z + reg_map[opr[0]] + "0";
	    		}
	    	}
	    	else {
	    		p = opt;
	    		if(opt[0] == '+'){
	    			p = opt.substr(1);
	    		}
	    		string arr[3];
	    		arr[0] = "00";
	    		arr[1] = "0";

	    		if(opcode_table.count(p)){
	    			arr[0] = opcode_table[p];
	    		}	

	    		// --------- N and I BITS SETTING -------------

	    		// IMMEDIATE ADDRESSING
	    		if(opr.size() && opr[0] == '#'){
	    			arr[0] = Add_Hex(arr[0],"1");
	    		}
	    		// INDIRECT ADDRESSING
	    		else if(opr.size() && opr[0] == '@'){
	    			arr[0] = Add_Hex(arr[0],"2");
	    		}
	    		// NEITHER IMMEDIATE NOR INDIRECT
	    		else {
	    			arr[0] = Add_Hex(arr[0],"3");
	    		}

	    		// --------- X, B, P, E BITS SETTING -----------

	    		// E BIT
	    		if(obj_size[line_no] == 4){
	    			arr[1] = Add_Hex(arr[1],"1");
	    		}
	    		// P BIT
	    		if(opr.size() && opr[0] != '#' && opt[0] != '+'){
	    			arr[1] = Add_Hex(arr[1],"2");
	    		}
	    		// X BIT
	    		if(opr.size() > 1){
	    			for(int i = 0; i < opr.size()-1; i++){
	    				if(opr[i] == ',' && opr[i+1] == 'X'){
	    					arr[1] = Add_Hex(arr[1],"8");
	    					break;
	    				}
	    			}
	    		}

	    		if(opt.size() && opt == "WORD"){
	    			arr[0] = "00";
	    			arr[1] = "0";
  	    		}

	    		// ------- ADDRESS CALCULATION ----------------

	    		arr[2] = "000";
	    		if(obj_size[line_no] == 4)
	    		{
	    			arr[2] = "00000";
	    			for(i = 0; i < opr.size(); i++){
	    				if(opr[i] == '+' || opr[i] == '-'){
	    					val = "";
	    					j = i+1;
	    					while(j < opr.size() && ((opr[j] >= '0' && opr[j] <= '9') || (opr[j] >= 'A' && opr[j] <= 'F'))){
	    						val.push_back(opr[j]);
	    						j++;
	    					}
	    					if(opr[i] == '+'){
	    						arr[2] = Add_Hex(arr[2],val);
	    					}
	    					else {
	    						arr[2] = subtract(arr[2],val);
	    						make_size(arr[2],5);
	    					}
	    					break;
	    				}
	    			}
	    			make_size(arr[2],5);
	    		}
	    		else 
	    		{
	    			if(opr.size() && opr[0] == '#')
	    			{
	    				add = "0";
	    				bool f = 1;
	    				string str;
	    				for(i = 1; i < opr.size()+1; i++)
	    				{
	    					if(i == opr.size() || opr[i] == '+' || opr[i] == '-')
	    					{
	    						for(auto it:label_to_address[str]){
	    							if(it.first == ctrl_section){
	    								str = it.second;
	    								break;
	    							}
	    						}
	    						if(f){
	    							add = Add_Hex(add,str);
	    						}
	    						else {
	    							add = subtract(add,str);
	    						}
	    						str = "";
	    						if(i != opr.size() && opr[i] == '-'){
	    							f = 0;
	    						}
	    						else {
	    							f = 1;
	    						}
	    					}
	    					else {
	    						str.push_back(opr[i]);
	    					}
	    				}
	    				arr[2] = Add_Hex(arr[2],add);
						make_size(arr[2],3);
	    			}
	    			else if(opr.size())
	    			{
	    				sub = addr_map[line_no+1];
	    				add = "0";

	    				chk = opr;
	    				if(chk[0] == '@') chk = chk.substr(1);
	    				for(auto it:label_to_address[chk]){
	    					if(it.first == ctrl_section){
	    						add = it.second;
	    						break;
	    					}
	    				}
	    				arr[2] = Add_Hex(arr[2],add);
	    				arr[2] = subtract(arr[2],sub);
	    				make_size(arr[2],3);
	    			} 
	    			make_size(arr[2],3);
	    		}
	    		z = arr[0] + arr[1] + arr[2];
	    	}
	    	
	    	// ADDING LITERAL CONSTANTS IN LITERAL TABLE
	    	if(opr.size() && opr[0] == '=')
	    	{
	    		p = opr.substr(3,opr.size()-4);
	   			if(!occur.count(opr)){
	   				if(p == "EOF"){
	   					literal_table.push_back({opr,3});
	   				}
	   				else {
	   					literal_table.push_back({opr,p.size()/2});
	   				}
	   				occur[opr] = 1;
	   			}
	    	}
	    	add_trailing_spaces(z);
	    	fi << z << "\n";
	    	line_no++;
    	}

    	// PROCESSING LITERAL TABLE AFTER END OF EACH CONTROL SECTION
    	for(auto it:literal_table)
    	{
		    addr = addr_map[line_no];
		    label = "*";
		    opt1 = it.first;
		    opr = "";
		    obj_code = opt1.substr(3,opt1.size()-4);
		    if(obj_code == "EOF"){
		    	obj_code = "454F46";
		    }
		    add_trailing_spaces(addr);
		    add_trailing_spaces(label);
		    add_trailing_spaces(opt1);
		    add_trailing_spaces(obj_code);
		    add_trailing_spaces1(opr);
		    fi << addr << label << opt1 << opr << obj_code;
		    if(!fp.eof()){
		    	fi << "\n";	
		    } 
			line_no++;
		}
		if(!fp.eof()){
			fi << "\n";
		}
		literal_table.clear();

		// INCREMENTING CONTROL SECTION NUMBER
    	ctrl_section++;
    }
    fp.close();
    fo.close();
    fi.close();
    fs.close();
	return 0;
}