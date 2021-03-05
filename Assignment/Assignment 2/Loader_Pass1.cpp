/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	g++ -o Pass2 180101097_Assign02_Pass2.cpp 
	./Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

struct ESTAB {
  	string csname, extsym, address, length;
};

int main()
{
	ESTAB es[100];
	string input, name, symbol;  
	string progaddr, csaddr, add, len;
	int count = 0, a;
	char ch;

	fstream fp1,fp2;
	fp1.open("object.txt",ios::in);
	fp2.open("estab.txt",ios::out);

	cout << "\nEnter the address where the program has to be loaded: ";

	cin >> progaddr; 		// TAKING THE PROGRAM ADDRESS FROM THE USER, GENERALLY IT IS DONE BY THE OS
	csaddr = progaddr;
	
	a = fp1.tellg();
	getline(fp1,input);
	fp1.seekg(a,ios::beg);

	while(!fp1.eof() && input != "END")
	{
		getline(fp1,input);
		if(!input.size()){
			continue;
		}
		if(input[0] == 'H')
		{
			stringstream str(input);
			string temp;
			vector<string> vtr;
			while(getline(str,temp,'^')){
				remove_trailing_spaces(temp);
				vtr.push_back(temp);
			}
			name = vtr[1];
			es[count].csname = name;
			es[count].extsym = "  ";
			
			add = vtr[2];
			es[count].address = Add_Hex(add, csaddr);
			
			len = vtr[3];
			es[count].length = len;
			fp2 << es[count].csname << " **** " << es[count].address << " " << es[count].length << "\n";
		}
		else if(input[0] == 'D')
		{
			stringstream str(input);
			string temp;
			vector<string> vtr;
			while(getline(str,temp,'^')){
				remove_trailing_spaces(temp);
				vtr.push_back(temp);
			}
			while(input[0] != 'R')
			{
				for(int i = 1; i < vtr.size()-1; i+=2){
					es[count].csname = "  ";
					es[count].extsym = vtr[i];
					es[count].address = Add_Hex(vtr[i+1],csaddr);
					es[count].length = "  ";
					fp2 << "**** " << es[count].extsym << " " << es[count].address << " ****\n";
					count++;
				}
				getline(fp1,input);
			}
		}
		else if(input[0] == 'T')
		{
			while(input[0] != 'E'){
				getline(fp1,input);
			}
			csaddr = Add_Hex(csaddr,len);
			len = "";
		}
	}
  	fp1.close();
  	fp2.close();
  	return 0;
}