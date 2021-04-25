/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	g++ -o Loader_Pass2 180101097_Assign02_Loader_Pass2.cpp 
	./Loader_Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

struct estab_table {
  	string ctrl_sec_name, symbol_name, address, length;
};

struct objectcode {
	string code, add;
};

int main()
{
	estab_table estab[2000];	// used to store estab values
 	objectcode obcode[100000];	// contains the corresponding object code from the starting address of program 

 	fstream fp1,fp2,fp3;

 	// fp1 is file pointer for object file
	// fp2 is file pointer for estab file
	// fp3 is file pointer for output file
 	
 	fp1.open("object.txt",ios::in);
 	fp2.open("estab.txt",ios::in);
 	fp3.open("output.txt",ios::out);

 	string pstart, label[5000], exeloc, loc, start, st, input, mloc[3000], textloc, textlen, lbl, newadd, address, s;
 	int i, j, x, y, length, location, loc1;
 	int n = 0, num = 0, inc = 0, count = 0, record = 0, mlen[1000];
 	char operation, opr[3000], ch, temp[1000];
 	
 	// storing values in estab table 
	while(!fp2.eof())
 	{
		fp2 >> estab[num].ctrl_sec_name >> estab[num].symbol_name >> estab[num].address >> estab[num].length;
		num++;
 	}
 	exeloc = loc = start = st = estab[0].address;

 	while(!fp1.eof())
 	{
		getline(fp1,input);
		if(!input.size() || input == "END"){
			continue;
		}

		// for header record
		if(input[0] == 'H')
		{
			stringstream str(input);
			string temp;
			vector<string> vtr;
			while(getline(str,temp,'^')){
				remove_trailing_spaces(temp);
				vtr.push_back(temp);
			}
			for(int i = 0; i < num; i++){
				if(vtr[1] == estab[i].ctrl_sec_name) {
					pstart = estab[i].address;
					break;
				}
			}
			while(!fp1.eof() && input[0] != 'T'){
				getline(fp1,input);
			}
		}
		do
		{
			// for text record
			if(input[0] == 'T')
			{
				stringstream str(input);
				string temp;
				vector<string> vtr;
				while(getline(str,temp,'^')){
					remove_trailing_spaces(temp);
					vtr.push_back(temp);
				}

				textloc = vtr[1];
				textloc = Add_Hex(textloc, pstart);

				string x = subtract(textloc,loc);
				int p = hexadecimalToDecimal(x);
				for(i = 0; i < p; i++)
				{
					// no instruction is present in this location
					obcode[inc].code = "..";
					obcode[inc++].add = start;
					start = Add_Hex(start,"1");
				}
				textlen = vtr[2];
				loc = Add_Hex(textloc, textlen);

				for(int j = 3; j < vtr.size(); j++){
					for(int k = 0; k < vtr[j].size()-1; k+=2){

						// writing instruction code and address
						obcode[inc].code = vtr[j].substr(k,2);
						obcode[inc++].add = start;
						start = Add_Hex(start,"1");
					}
				}
		 	}
		 	// for modification records
			else if(input[0] == 'M')
			{
				stringstream str(input);
				string temp;
				vector<string> vtr;
				while(getline(str,temp,'^')){
					remove_trailing_spaces(temp);
					vtr.push_back(temp);
				}

				// storing modification records
				mloc[record] = Add_Hex(vtr[1], pstart);
				mlen[record] = hexadecimalToDecimal(vtr[2]);
				label[record] = vtr[3];
				record++;
			}
			getline(fp1,input);
		} 
		while(input[0] != 'E');
 	}

 	// processing modification records
	for(n = 0; n < record; n++)
	{
		operation = label[n][0];
		length = label[n].size();
		
		lbl = label[n].substr(1);

	 	length = 0;
		address = "";
		location = hexadecimalToDecimal(subtract(mloc[n], exeloc));
		loc1 = location;
		count = 0;

		// creating address for this current modification record
		while(length < mlen[n])
		{
			address += obcode[location++].code;
			count++;
			length += 2;
		}

		// checking for modification record label in estab table
		for(i = 0; i < num; i++)
		{
			if(lbl == estab[i].ctrl_sec_name || lbl == estab[i].symbol_name){
				break;
			}
		}

		// creating new address to write 
		switch(operation)
		{
			case '+':
				newadd = Add_Hex(address,estab[i].address);
				break;
			case '-':
				newadd = subtract(address,estab[i].address);
				break;
		}
		while(newadd.size() < length){
			newadd = "0" + newadd;
		}
		if(newadd.size() > length){
			int p = newadd.size() - length;
			newadd = newadd.substr(p);
		}
  		x = 0; 
  		y = 0;
  		while(count > 0)
  		{
  			// writing new address value in location
   			obcode[loc1].code[x++] = newadd[y++];
   			if(x > 1)
   			{
				x = 0; 
				loc1++;
				count--;
   			}
  		}
  	}

  	// writing addresses and their corresponding location values in output.txt
 	fp3 << "Address" << "\t\t" << "Value\n\n";
 	for(i = 0; i < inc; i++){
 		if(obcode[i].code != ".."){
 			fp3 << obcode[i].add << "\t\t";
			fp3 << obcode[i].code << "\n";
	 	}
  	}
	
	fp1.close();
	fp2.close();
	fp3.close();
 	return 0;
}