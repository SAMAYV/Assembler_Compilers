#include <bits/stdc++.h>
using namespace std;

int length(string n){
    while(n.size() && n.back() == ' '){
        n.pop_back();   
    } 
    return n.size();
}

int find_number_of_columns(string ch){
    int intervals = 0;
    for(int i = 0; i < ch.size(); i++){
        if(ch[i] == ' ' && ch[i+1] != ' '){
            intervals++;
        }
    }
    return intervals - 1;
}

void remove_trailing_spaces(string& a){
    while(a.size() && a.back() == ' '){
        a.pop_back();
    }
}

void add_trailing_spaces(string& a){
    while(a.size() < 10){
        a.push_back(' ');
    }
}
  
// Function to add the two hexadecimal numbers 
string Add_Hex(string a, string b) 
{ 
    map<char,int> m = {{ '0', 0 }, { '1', 1 }, { '2', 2 }, { '3', 3 }, { '4', 4 }, { '5', 5 }, { '6', 6 }, { '7', 7 },  
                        { '8', 8 }, { '9', 9 }, { 'A', 10 }, { 'B', 11 }, { 'C', 12 }, { 'D', 13 }, { 'E', 14 }, { 'F', 15 } }; 
    map<int,char> k = {{ 0, '0' }, { 1, '1' }, { 2, '2' }, { 3, '3' }, { 4, '4' }, { 5, '5' }, { 6, '6' }, { 7, '7' },  
                        { 8, '8' }, { 9, '9' }, { 10, 'A' }, { 11, 'B' }, { 12, 'C' }, { 13, 'D' }, { 14, 'E' }, { 15, 'F' } }; 
   
    if(a.length() < b.length()){
        swap(a, b);
    }  
    int l1 = a.size(), l2 = b.size(); 
    string ans = ""; 
  
    int carry = 0, i, j; 
  
    // Traverse till second string get traversal completely 
    for(i = l1 - 1, j = l2 - 1; j >= 0; i--, j--){ 
        int sum = m[a[i]] + m[b[j]] + carry;  
        int addition_bit = k[sum % 16];  
        ans.push_back(addition_bit);  
        carry = sum / 16; 
    } 
  
    // Traverse remaining element of string a 
    while(i >= 0){  
        int sum = m[a[i]] + carry;  
        int addition_bit = k[sum % 16]; 
        ans.push_back(addition_bit);  
        carry = sum / 16; 
        i--; 
    } 

    if(carry){ 
        ans.push_back(k[carry]); 
    }  
    reverse(ans.begin(), ans.end()); 
    return ans; 
} 

// function to convert decimal to hexadecimal
string decToHexa(int n) 
{     
    string hexaDeciNum;  
    int i = 0; 
    while(n != 0){     
        int temp = n % 16;  
        if(temp < 10){ 
            hexaDeciNum.push_back(temp + 48); 
            i++; 
        } 
        else { 
            hexaDeciNum.push_back(temp + 55); 
            i++; 
        } 
        n = n / 16; 
    } 
    reverse(hexaDeciNum.begin(),hexaDeciNum.end());
    return hexaDeciNum;
} 

// function to convert hexadecimal to decimal
int hexadecimalToDecimal(string hexVal) 
{    
    int len = hexVal.size();  
    int base = 1;  
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for(int i=len-1; i>=0; i--) 
    {     
        if(hexVal[i] >= '0' && hexVal[i] <= '9'){ 
            dec_val += (hexVal[i] - 48)*base; 
            base = base * 16; 
        }  
        else if(hexVal[i] >= 'A' && hexVal[i] <= 'F'){ 
            dec_val += (hexVal[i] - 55)*base; 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 

// subtract 2 hexadecimal values
string subtract(string a,string b){
    int v = hexadecimalToDecimal(a) - hexadecimalToDecimal(b);
    return decToHexa(v);
}