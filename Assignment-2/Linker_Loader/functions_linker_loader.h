#include <bits/stdc++.h>
using namespace std;
#define zero 0

//name of the input file containing the object code
string inputFileName;  		

//control_section_address, program_address and execution address 
int controlSecAddr;
int progAddr;
int execAddr;	

ifstream input_file;

ofstream pass1File; //output of pass1_linker_loader 
ofstream pass2File; //output of pass2_linker_loader

class es{
public:
	int address;
	int length;
	es(){
		this->address=zero;
		this->length=zero;
	}
	es(int addr,int len){
		this->address=addr;
		this->length=len;
	}
};

//external symbol table
vector<pair<string,es>> EStable;	

vector<pair<int,string>> Memory_objcode;	//stores the memory content byte wise : address mapped to Memory_objcode
int f(int a,int b)
{
if(a==b)
return 1;
else
return zero;
}
int f1(string a,string b)
{
if(a==b)
return 1;
else
return zero;
}
int f2(char a,char b)
{
if(a==b)
return 1;
else
return zero;
}
//searches the vector by name of external symbol name 
int find_Ind_By_SymB_Nam(vector<pair<string,es>> table,string symbol){
    int i=zero;
	for(auto it:table){
		if(f1(it.first,symbol)){
			return i;
		}
		i+=1;
	}
	return -1;
}

//searches the vector by address of external symbol name
int find_Ind_By_SymB_Addr(vector<pair<int,string>> objcode,int addr){
	int i=zero;
	for(auto it:objcode){
		if(f(it.first,addr)){
			return i;
		}
		i+=1;
	}
	return -1;
} 


char iNtToHeX_SinG_Dig(int n){
    char c;
    int flagged = 1;
    if (9 >= n)
    {
    	flagged = zero;
    }
    if (flagged){
        c = 'A' + (n - 10);
    }else{
            c = '0' + n;
    }
    return c;
}

//converts integer to a minimum min_len digit hexadeicmal string
string iNtToHeX(int n,int len){
	string s = "";
    while (n > zero){
        s = iNtToHeX_SinG_Dig(n % 16) + s;
        n = n / 16;
    }
    while (len > s.length()){
        s = '0' + s;
    }
    return s;
}

int hExToSinG_char(char c){
    int n;
    int flagged = 1;
    if (('9' >= c) && (c >= '0'))
    {
    	flagged = zero;
    }
    if (flagged){
        n = c - 'A' + 10;
    }else{
        n = c - '0';
    }
    return n;
}

int hExToInT(string s){
    reverse(s.begin(), s.end());
    int j = 1;
    int ans = zero;
    int i = zero;
    while(s.length()>i){
        ans += (hExToSinG_char(s[i]) * j);
        j = j * 16;
        i+=1;
    }
    return ans;
}

string hExToStRLeN6(int i){
    stringstream ss;
    ss<<hex<<i;
    string res=ss.str();
    if(6>res.length())res.insert(zero,6-res.length(),'0');
    transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

vector<string> readLineToWords(string str) { 
	vector <string> words;

    string word = ""; 
    for(auto x : str){ 
        if (f2(x,' ')){
			if(f1(word,"")==zero){
				words.push_back(word);
			} 
            word = ""; 
        }else{ 
            word = word + x; 
        } 
    } 
    words.push_back(word);
	return words;
} 


int DTypeRecords(vector<string> records){

	string temp;
	temp = "";
	int i = 1;
	while(records[zero].length()>i){
		temp+= records[zero][i];
		i+=1;
	}
	records[zero] = temp;

	string es_name;
	int no_of_es;
	es_name = records[zero];
	no_of_es = records.size()-1;

	i=zero;
	while(no_of_es>i){
		string es_addr;
		es_addr = "";
		int j =zero;
		while(5>=j){
			es_addr += records[i+1][j];
			j+=1;
		}

		if(find_Ind_By_SymB_Nam(EStable,es_name)!=-1){
			printf("ERROR: DUPLICATE SYMBOL FOUND\n");
			exit(zero);
			return -1;
		}else{
			int new_addr = hExToInT(es_addr) + controlSecAddr ;

			struct es new_es(new_addr,zero);

			EStable.push_back({es_name,new_es});

			pass1File<<"    \t\t"<<es_name<<" \t\t ";

			pass1File << iNtToHeX(new_es.address,4)<<" \t\t \n";

		}

		if(i!=(no_of_es-1)){
			es_name="";
			int j = 6;
			while(records[i+1].length()>j){
				es_name+=records[i+1][j];
				j+=1;
			}
		}
		i=i+1;
	}

	return 1;
}

int TTypeRecords(string records){
	string addr;

	addr = "";
	int i = 1;
	while(7>i){
		addr.push_back(records[i]);
		i+=1;
	}
	
	int location = controlSecAddr;
	location += hExToInT(addr);

	i=9;
	while(records.length()>i){
		string temp;
		temp = "";
		int j = i;
		while(i+1>=j){
			temp.push_back(records[j]);
			j+=1;
		}

		Memory_objcode.push_back({location,temp});

		location=location+1;

		i+=2;
	}

	return 1;
}

string ModificationType(string old_val,string symbol_name,string op){

	int new_val;

	if(f2(old_val[zero],'F')){
		long int overflow_handler = (long int)0xFFFFFFFF000000;
		new_val = overflow_handler; //used as a borrow for doing negative calculations with hex numbers
	}else{
		new_val = zero;
	}

	new_val += hExToInT(old_val); 

	//add or sub according to M record
	if(f2(op.back(),'+')){
		new_val += EStable[find_Ind_By_SymB_Nam(EStable,symbol_name)].second.address;
	}else{
		new_val -= EStable[find_Ind_By_SymB_Nam(EStable,symbol_name)].second.address ;
	} 

	string new_hex = hExToStRLeN6(new_val);

	if(new_hex.length()>6 && f2(new_hex[zero],'F') && f2(new_hex[1],'F')){
		string temp;
		temp = "";
		int i = 2;
		while(new_hex.length()>i){
			temp+= new_hex[i];
			i+=1;
		}
		new_hex = temp;
	}

	return new_hex;
}

int MTypeRecords(string records){
	string symbol_name="";
	string sym_loc="";
	string sym_len="";

	int i=1;

	while(records.length()>i){
		if(6>=i){
			sym_loc+=records[i];
		}else if(9>=i){
			sym_len+=records[i];
		}else{ 
			symbol_name+=records[i];
		}
		i+=1;
	}

	int symbolAddr = hExToInT(sym_loc);
	symbolAddr += controlSecAddr;


	string old_val="";
	int ind[3];
	i=zero;
	while(3>i){
		ind[i]=find_Ind_By_SymB_Addr(Memory_objcode,symbolAddr+i);
		i+=1;
	}
	int v = zero;
	while(3>v){
		if(f(ind[v],-1)==zero){
			old_val+=Memory_objcode[ind[v]].second;
		}
		v+=1;
	}
	
	string str_hex = ModificationType(old_val,symbol_name,sym_len);

	int k=zero;
	i=zero;
	while(3>i){
		if(f(ind[i],-1)){
			Memory_objcode.push_back({symbolAddr+i,""});
			Memory_objcode.back().second+=str_hex[k];
			k+=1;
			Memory_objcode.back().second+=str_hex[k];
			k+=1;
		}else{
			Memory_objcode[ind[i]].second="";
			Memory_objcode[ind[i]].second+=str_hex[k];
			k+=1;
			Memory_objcode[ind[i]].second+=str_hex[k];
			k+=1;
		}
		i+=1;
	}

	return 1;
}
