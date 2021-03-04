/*  
    This Program should be compiled and executed in LINUX g++ environment 
    g++ -o Pass2 180101097_Assign02_Pass2.cpp 
    ./Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

int main()
{
    fstream fp,fi,fo,ff;
    string counter1, curr_addr, start_addr, end_addr, curr_opt, curr_label, program_name, obj_code, ctr, prog_start, prog_size, line, addr, addr1, label, opt, opr, opt1, opvalue, p;
    int counter, d, iter = 0, size, a, b, start, j, l, k, intervals, i, temp, n;

    fp.open("intermediate_1.txt",ios::in);
    ff.open("object_1.txt",ios::out);

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

    a = fp.tellg();
    while(!fp.eof()){
        getline(fp,line);
        if(!line.size() || line[0] == '.'){
            continue;
        }
        string p = line.substr(20,10);
        remove_trailing_spaces(p);
        if(p == "END"){
            end_addr = line.substr(30,30); 
            remove_trailing_spaces(end_addr);
            break;
        }   
    }

    fp.seekg(a,ios::beg);
    while(!fp.eof())
    {
        bool end = 0;
        map<string,string> label_to_addr;

        // WRITING HEADER RECORD-----------------------------------------------------------------------------
        
        getline(fp,line);
        a = fp.tellg();

        if(line.size() == 0 || line[0] == '.'){
            continue;
        }

        addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,30);
        obj_code = line.substr(60,10);

        remove_trailing_spaces(addr);
        remove_trailing_spaces(label);
        remove_trailing_spaces(opt);
        remove_trailing_spaces(opr);
        remove_trailing_spaces(obj_code);

        ctr = addr;
        counter = stoi(addr);
        prog_start = addr;
        program_name = label;
        
        if(opt == "START" || opt == "CSECT")
        {
            ff << "H^" << label;
            for(k = label.size(); k < 6; k++){
                ff << " ";
            }
            if(label.size()){
                label_to_addr[label] = addr;
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
                    string curr_obj_code = line.substr(60,10);
                    remove_trailing_spaces(curr_opt);
                    remove_trailing_spaces(curr_addr);
                    remove_trailing_spaces(curr_obj_code);

                    addr = max_calc(addr,Add_Hex(curr_addr,decToHexa(curr_obj_code.size()/2)));
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

        map<string,int> curr_ref;

        addr = line.substr(0,10);
        label = line.substr(10,10);
        opt = line.substr(20,10);
        opr = line.substr(30,30);
        obj_code = line.substr(60,10);

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
                    curr_ref[x] = 1;
                    x = "";
                }
                else {
                    x.push_back(opr[z]);
                }
            }
            if(x.size()){
                mp[x] = 1;
                curr_ref[x] = 1;
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
        opr = line.substr(30,30);
        obj_code = line.substr(60,10);

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

        vector<vector<string>> modification;

        while(!fp.eof())
        {
            a = fp.tellg();
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
                obj_code = line.substr(60,10);

                remove_trailing_spaces(addr);
                remove_trailing_spaces(opt);
                remove_trailing_spaces(obj_code);

                if(opt == "RESB" || opt == "RESW" || opt == "CSECT" || opt == "EQU"){
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

            //  WRITING TEXT RECORD-------------------------------------------------------------------------

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
                opr = line.substr(30,30);
                obj_code = line.substr(60,10);

                remove_trailing_spaces(addr);
                remove_trailing_spaces(label);
                remove_trailing_spaces(opt);
                remove_trailing_spaces(opr);
                remove_trailing_spaces(obj_code);

                if(label.size()){
                    label_to_addr[label] = addr;
                }

                // MODIFICATION RECORD CALCULATION
                vector<string> v;
                string str;
                int sign = 1, extsign = 0, ct = 0;
                for(int z = 0; z < opr.size(); z++){
                    if(opr[z] != '+' && opr[z] != '-' && opr[z] != ',' && opr[z] != '(' && opr[z] != ')' && opr[z] != '#'){
                        str.push_back(opr[z]);
                    }
                    if(opr[z] == ')' || opr[z] == '-' || opr[z] == '+' || z == opr.size()-1){
                        if(ext_ref.count(str)){
                            if(sign){
                                v.push_back("+" + str);
                            }
                            else {
                                v.push_back("-" + str);
                            }
                        }
                        else if(curr_ref.count(str) && opt == "WORD"){
                            if(sign){
                                ct++;
                            }
                            else {
                                ct--;
                            }                            
                        }
                        str = "";
                    }
                    if(opr[z] == '-' || opr[z] == '+'){
                        if(!extsign){
                            if(opr[z] == '-'){
                                sign = 0;
                            }
                            else {
                                sign = 1;
                            }
                        }
                        else {
                            if(opr[z] == '-'){
                                sign = 1;
                            }   
                            else {
                                sign = 0;
                            }
                        }
                    }
                    if(opr[z] == '('){
                        extsign ^= 1;
                        if(extsign){
                            sign = 0;
                        }
                        else {
                            sign = 1;
                        }
                    }
                    if(opr[z] == ')'){
                        extsign ^= 1;
                    }
                }
                if(ct > 0){
                    for(i = 0; i < ct; i++){
                        v.push_back("+" + program_name);
                    }
                }
                else {
                    for(i = 0; i < abs(ct); i++){
                        v.push_back("-" + program_name);
                    }   
                }
                int c = 5;
                int k; 
                if(opt == "WORD"){
                    c = 6;
                    k = hexadecimalToDecimal(addr);
                }
                else {
                    k = hexadecimalToDecimal(addr) + 1;
                }

                string p1 = decToHexa(c);
                while(p1.size() < 2){
                    p1 = "0" + p1;
                }
                string p2 = decToHexa(k);
                while(p2.size() < 6){
                    p2 = "0" + p2;
                }
                for(string it : v){
                    modification.push_back({p2, p1, it});
                }

                // increasing j
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
                opr = line.substr(30,30);
                remove_trailing_spaces(addr);
                remove_trailing_spaces(opt);
                remove_trailing_spaces(label);
                remove_trailing_spaces(opr);
                if(opt == "END"){
                    fp.seekg(a,ios::beg);
                    end = 1;
                    break;
                }
                if(opt == "RESB" || opt == "RESW" || opt == "LTORG" || opt == "EQU"){
                    continue;
                }
                else {
                    fp.seekg(a,ios::beg);
                    break;
                }
            }
            if(end){
                break;
            }
        }

        // FOR MODIFICATION RECORD
        for(vector<string> v : modification){
            ff << "\nM";
            for(string p : v){
                ff << "^" << p;
            }
        }

        // WRITING END RECORD--------------------------------------------------------------
        if(end || !iter){
            getline(fp,line);
            opr = line.substr(30,30);
            remove_trailing_spaces(opr);
            ff << "\nE";
            if(iter == 0){
                ff << "^00" << label_to_addr[end_addr] << "\n";
            }
            else if(opr.size()){
                ff << "^00" << label_to_addr[opr] << "\n";
            }
            else {
                ff << "\n";
            }    
        }
        else {
            ff << "\nE\n";
        }

        if(!fp.eof()){
            fp.seekg(d,ios::beg);
            ff << "\n";
        }
        iter++;
    }
    ff << "END";
    fp.close();
    ff.close();
    return 0;
}