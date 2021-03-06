/*  
    This Program should be compiled and executed in LINUX g++ environment 
    g++ -o SICXE_Pass2 SICXE_Pass2.cpp 
    ./SICXE_Pass2 
*/

#include <bits/stdc++.h>
#include <fstream>
#include "180101097_Assign02_Functions.h"

using namespace std;

int main()
{
    fstream fp,ff,fs;
    string counter1, curr_addr, start_addr, end_addr, curr_opt, curr_label, program_name, obj_code, ctr, prog_start, prog_size, line, addr, addr1, label, opt, opr, opt1, opvalue, p, str1, str2;
    int d, ctrl_section = 0, size, a, b, start, j, l, k, intervals, i, temp, n, f = 1, end = 0;

    fp.open("intermediate.txt",ios::in);
    ff.open("object.txt",ios::out);
    fs.open("symbol_table.txt",ios::in);

    map<pair<int,string>,string> symbol_table;
    map<string,string> label_to_addr;
    map<string,string> define_records;
    map<string,int> refer_records;
    vector<vector<string>> modification;

    // -------------------- BUILDING SYMBOL TABLE -----------------------------------------------------------

    while(!fs.eof())
    {
        getline(fs,line);
        if(!line.size()){
            ctrl_section++;
            continue;
        }
        str1 = line.substr(0,10);
        str2 = line.substr(10,10);
        remove_trailing_spaces(str1);
        remove_trailing_spaces(str2);
        if(str1 == "END" && f){
            f = 0;
            end_addr = str2; 
        }   
        symbol_table[{ctrl_section,str1}] = str2;
    }

    // ---------------------- WRITING OBJECT CODE -----------------------------------------------------------

    ctrl_section = 0;
    while(!fp.eof())
    {
        end = 0;
        label_to_addr.clear();

        // ------------------ WRITING HEADER RECORD-------------------------------------------------------------------------
        
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

            make_size(ctr,6);
            ff << "^" << ctr;
            curr_opt = "";
            while(!fp.eof() && curr_opt != "CSECT")
            {
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
            make_size(prog_size,6);
            ff << "^" << prog_size << "\n";

            // move back the file pointer to second line of intermediate file
            fp.seekg(a,ios::beg);
        }

        // ------------------------ FOR DEFINE RECORDS -------------------------------------------------------------------------

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

        map<string,int> curr_ref;
        define_records.clear();

        if(opt == "EXTDEF"){
            string x = "";
            for(int z = 0; z < opr.size()+1; z++){
                if(z == opr.size() || opr[z] == ','){
                    define_records[x] = "";
                    curr_ref[x] = 1;
                    x = "";
                }
                else {
                    x.push_back(opr[z]);
                }
            }
            for(auto& it:define_records){
                it.second = symbol_table[{ctrl_section,it.first}];
            }

            // WRITING DEFINE RECORDS FOR CURRENT CONTROL SECTION
            ff << "D";
            for(auto &it:define_records){
                make_size(it.second,6);
                ff << "^" << it.first << "^" << it.second;
            }
            ff << "\n";
        }
        else {
            fp.seekg(a,ios::beg);
        }

        // ------------------------ FOR REFER RECORDS -------------------------------------------------------------------------

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

        refer_records.clear();

        // WRITING REFER RECORDS FOR CURRENT CONTROL SECTION
        if(opt == "EXTREF"){
            ff << "R";
            string x = "";
            for(int z = 0; z < opr.size()+1; z++){
                if(z == opr.size() || opr[z] == ','){
                    ff << "^" << x;
                    refer_records[x] = 1;
                    for(int z = x.size(); z < 6; z++){
                        ff << " ";
                    }
                    x = "";
                }
                else {
                    x.push_back(opr[z]);
                }
            }
        }
        else {
            fp.seekg(a,ios::beg);
        }


        // --------------------- FOR TEXT RECORDS ---------------------------------------------------------------------------------

        modification.clear();

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

            // ------------------------ CALCULATING TEXT RECORD ROW SIZE ------------------------------------------------------------
            
            size = 0;
            start_addr = line.substr(0,10);
            remove_trailing_spaces(start_addr);

            fp.seekg(a,ios::beg);
            start = hexadecimalToDecimal(start_addr);

            while(!fp.eof())
            {
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

                if(opt == "END"){
                    end = 1;
                }
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

            // ----------------------- WRITING TEXT RECORD-------------------------------------------------------------------------

            fp.seekg(a,ios::beg);
            p = decToHexa(size);
            make_size(start_addr,6);

            if(size < 16){
                ff << "\nT^" << start_addr << "^0" << p;  
            }
            else {
                ff << "\nT^" << start_addr << "^" << p;   
            }

            j = 0;
            while(!fp.eof() && j < size)
            {
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

                // ---------------- MODIFICATION RECORD CALCULATION --------------------------------------------------------

                vector<string> v;
                string str;
                int sign = 1, extsign = 0, ct = 0;
                for(int z = 0; z < opr.size(); z++)
                {
                    if(opr[z] != '+' && opr[z] != '-' && opr[z] != ',' && opr[z] != '(' && opr[z] != ')' && opr[z] != '#'){
                        str.push_back(opr[z]);
                    }
                    if(opr[z] == ')' || opr[z] == '-' || opr[z] == '+' || opr[z] == ',' || z == opr.size()-1){
                        if(refer_records.count(str)){
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
                make_size(p1,2);

                string p2 = decToHexa(k);
                make_size(p2,6);

                for(string it : v){
                    modification.push_back({p2, p1, it});
                }

                // increasing j
                j += obj_code.size()/2;
                if(obj_code.size()){
                    ff << "^" << obj_code;
                }
            }

            // ---------------- SCANNING CONSECUTIVE RESB OR RESW DEFINING INSTRUCTIONS WHICH DON'T HAVE ANY OBJECT CODE ------------
            
            while(!fp.eof())
            {
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

        // -------------------- WRITING MODIFICATION RECORD FOR CURRENT CONTROL SECTION -------------------------------------

        for(vector<string> v : modification){
            ff << "\nM";
            for(string p : v){
                ff << "^" << p;
            }
        }

        // ------------------------- WRITING END RECORD-----------------------------------------------------------------------
        
        if(!ctrl_section){
            make_size(label_to_addr[end_addr],6);
            ff << "\nE^" << label_to_addr[end_addr] << "\n";
        }
        else if(end){
            getline(fp,line);
            opr = line.substr(30,30);
            remove_trailing_spaces(opr);
            if(symbol_table.count({ctrl_section,opr}))
            {
                make_size(label_to_addr[symbol_table[{ctrl_section,opr}]],6);
                ff << "\nE^" << label_to_addr[symbol_table[{ctrl_section,opr}]] << "\n";
            } 
            else {
                ff << "\nE\n";
            }
        }
        else {
            ff << "\nE\n";
        }

        if(!fp.eof()){
            fp.seekg(d,ios::beg);
            ff << "\n";
        }

        // INCREMENTING CONTROL SECTION NUMBER
        ctrl_section++;
    }
    ff << "END";
    fp.close();
    ff.close();
    fs.close();
    return 0;
}