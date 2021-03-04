/* 	
	This Program should be compiled and executed in LINUX g++ environment 
	g++ -o Pass2 180101097_Assign02_Loader_Pass2.cpp 
	./Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

struct exttable {
  	string cextsym, extsym, address, length;
};

struct objectcode {
	string code, add;
};

int main()
{
	exttable estab[2000];
 	objectcode obcode[50000];

 	fstream fp1,fp2,fp3;
 	fp1.open("object.txt",ios::in);
 	fp2.open("estab.txt",ios::in);
 	fp3.open("output.txt",ios::out);

 	string pstart, label[5000], exeloc, loc, start, st, input, mloc[3000], textloc, textlen, lbl, newadd, address, s;
 	int i,j,x,y,length,location,loc1;
 	int n = 0, num = 0, inc = 0, count = 0, record = 0, mlen[1000];
 	char operation, opr[3000], ch, *add1, temp[1000];
 	

	while(!fp2.eof())
 	{
		fp2 >> estab[num].cextsym >> estab[num].extsym >> estab[num].address >> estab[num].length;
		num++;
 	}
 	exeloc = loc = start = st = estab[0].address;

 	while(!fp1.eof())
 	{
		getline(fp1,input);
		if(!input.size() || input == "END"){
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
			for(int i = 0; i < num; i++){
				if(vtr[1] == estab[i].cextsym) {
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
					obcode[inc].code = "..";
					obcode[inc++].add = start;
					start = Add_Hex(start,"1");
				}
				textlen = vtr[2];
				loc = Add_Hex(textloc, textlen);

				for(int j = 3; j < vtr.size(); j++){
					for(int k = 0; k < vtr[j].size()-1; k+=2){
						obcode[inc].code = vtr[j].substr(k,2);
						obcode[inc++].add = start;
						start = Add_Hex(start,"1");
					}
				}
		 	}
			else if(input[0] == 'M')
			{
				stringstream str(input);
				string temp;
				vector<string> vtr;
				while(getline(str,temp,'^')){
					remove_trailing_spaces(temp);
					vtr.push_back(temp);
				}
				mloc[record] = Add_Hex(vtr[1], pstart);
				mlen[record] = hexadecimalToDecimal(vtr[2]);
				label[record] = vtr[3];
				record++;
			}
			getline(fp1,input);
		} 
		while(input[0] != 'E');
 	}

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

		while(length < mlen[n])
		{
			address += obcode[location++].code;
			count++;
			length += 2;
		}
		for(i = 0; i < num; i++)
		{
			if(lbl == estab[i].cextsym || lbl == estab[i].extsym){
				break;
			}
		}
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
   			obcode[loc1].code[x++] = newadd[y++];
   			if(x > 1)
   			{
				x = 0; 
				loc1++;
				count--;
   			}
  		}
  	}
 	
 	count = 0;
 	n = 0;
 	s = subtract(st,"10");
 	while(s.size() < 6){
 		s = "0" + s;
 	}
 	fp3 << s << "\t";
 	for(i = 1; i <= 16; i++)
 	{
  		fp3 << "xx";
  		if(i % 4 == 0){
   			fp3 << "\t";
  		}
 	}
 	fp3 << "\n\n" << obcode[0].add << "\t";
 	for(i = 0; i < inc; i++)
 	{
		fp3 << obcode[i].code;
		n++;
		if(n > 3)
		{
			fp3 << "\t";
			n = 0;
			count++;
		}
		if(count > 3)
		{
			fp3 << "\n\n" << obcode[i+1].add << "\t";
			count = 0;
		}
  	}
	
	fp1.close();
	fp2.close();
	fp3.close();
 	return 0;
}