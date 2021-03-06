/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	g++ -o Loader_Pass1 180101097_Assign02_Loader_Pass1.cpp 
	./Loader_Pass1 <address>
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

struct ESTAB {
  	string ctrl_sec_name, symbol_name, address, length;
};

int main(int argc, char* argv[])
{
	ESTAB es[1000];
	string line, name;  
	string prog_addr, ctrl_sec_addr, add, len;
	int count = 0, a;
	char ch;

	fstream fp1,fp2;
	fp1.open("object.txt",ios::in);
	fp2.open("estab.txt",ios::out);

	// TAKING THE PROGRAM ADDRESS FROM THE USER, GENERALLY IT IS DONE BY THE OS
	if(argc > 1){
		prog_addr = string(argv[1]); 	
	}
	else {
		prog_addr = "1000";
	}

	ctrl_sec_addr = prog_addr;
	
	a = fp1.tellg();
	getline(fp1,line);
	fp1.seekg(a,ios::beg);

	while(!fp1.eof() && line != "END")
	{
		getline(fp1,line);
		if(!line.size() || line[0] == '.'){
			continue;
		}
		if(line[0] == 'H')
		{
			stringstream str(line);
			string temp;
			vector<string> vtr;
			while(getline(str,temp,'^')){
				remove_trailing_spaces(temp);
				vtr.push_back(temp);
			}
			name = vtr[1];
			es[count].ctrl_sec_name = name;
			es[count].symbol_name = "  ";
			
			add = vtr[2];
			es[count].address = Add_Hex(add, ctrl_sec_addr);
			
			len = vtr[3];
			es[count].length = len;
			fp2 << es[count].ctrl_sec_name << " ***** " << es[count].address << " " << es[count].length << "\n";
		}
		else if(line[0] == 'D')
		{
			stringstream str(line);
			string temp;
			vector<string> vtr;
			while(getline(str,temp,'^')){
				remove_trailing_spaces(temp);
				vtr.push_back(temp);
			}
			while(line[0] != 'R')
			{
				for(int i = 1; i < vtr.size()-1; i+=2){
					es[count].ctrl_sec_name = "  ";
					es[count].symbol_name = vtr[i];
					es[count].address = Add_Hex(vtr[i+1],ctrl_sec_addr);
					es[count].length = "  ";
					fp2 << "***** " << es[count].symbol_name << " " << es[count].address << " *****\n";
					count++;
				}
				getline(fp1,line);
			}
		}
		else if(line[0] == 'T')
		{
			while(line[0] != 'E'){
				getline(fp1,line);
			}
			ctrl_sec_addr = Add_Hex(ctrl_sec_addr,len);
			len = "";
		}
	}
  	fp1.close();
  	fp2.close();
  	return 0;
}