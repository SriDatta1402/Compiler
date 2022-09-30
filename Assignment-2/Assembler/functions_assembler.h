#include <bits/stdc++.h>
using namespace std;

const int MAXS = 1024;
const int MAXW = 10;

const int ILEN = 1 ;
const int WLEN = 1 ;

const char *symtabFile = "symbolTable.txt";

//List of Registers
unordered_map <string, string> register_list = {
    {"A", "0"},
    {"X", "1"},
    {"B", "3"},
    {"S", "4"},
    {"T", "5"},
    {"F", "6"}
};

int ERROR_FLAG = 0;

int PROGLEN;         // store program length
string PROGNAME; // store program length
int STARTADDR;       // store program length

class op_code{
public:
    string op_addr;
    int format;
};

//Operation Code Table
unordered_map <string, op_code> OPTAB;

void load_OPTAB(){
    OPTAB["LDA"] = {"00",3};
    OPTAB["LDX"] = {"04",3};
    OPTAB["LDL"] = {"08",3};
    OPTAB["LDB"] = {"68",3};
    OPTAB["LDT"] = {"74",3};
    OPTAB["STA"] = {"0C",3};
    OPTAB["STX"] = {"10",3};
    OPTAB["STL"] = {"14",3};
    OPTAB["LDCH"] = {"50",3};
    OPTAB["STCH"] = {"54",3};
    OPTAB["ADD"] = {"18",3};
    OPTAB["SUB"] = {"1C",3};
    OPTAB["MUL"] = {"20",3};
    OPTAB["DIV"] = {"24",3};
    OPTAB["COMP"] = {"28",3};
    OPTAB["COMPR"] = {"A0",2};
    OPTAB["CLEAR"] = {"B4",2};
    OPTAB["J"] = {"3C",3};
    OPTAB["JLT"] = {"38",3};
    OPTAB["JEQ"] = {"30",3};
    OPTAB["JGT"] = {"34",3};
    OPTAB["JSUB"] = {"48",3};
    OPTAB["RSUB"] = {"4C",3};
    OPTAB["TIX"] = {"2C",3};
    OPTAB["TIXR"] = {"B8",2};
    OPTAB["TD"] = {"E0",3};
    OPTAB["RD"] = {"D8",3};
    OPTAB["WD"] = {"DC",3};
}

op_code* search_optab(string opcode){
	if (OPTAB.find(opcode) != OPTAB.end()){
		return &OPTAB[opcode];
	}
	return NULL;
}

class symtab{
public:
    map<string, string> table;
    set<string> extref;
    set<string> extdef;
    int length;

    symtab(){}

    string search_symtab(string label){
        if (table.find(label) != table.end()){
            return table[label];
        }
        return "";
    }
    
    void insert_symtab(string label, int locctr){
        FILE *filesymtab = fopen(symtabFile, "a");
        char addr[MAXW];
        sprintf(addr, "%0X", locctr);
        table[label] = {addr};
        fprintf(filesymtab, "%s\t%s\n", addr, label.c_str());
        fclose(filesymtab);
    }
};

map<string, symtab *> symtab_list;
set<string> literal_pool;

struct modrec {
    int addr;
    int length;
    bool sign;
    string symbol;
};
int f(int a,int b)
{
if(a==b)
return 1;
else
return 0;
}
int f1(string a,string b)
{
if(a==b)
return 1;
else
return 0;
}
int f2(char a,char b)
{
if(a==b)
return 1;
else
return 0;
}
int read_Line(char *str, int start, char *words[], string delimiter){
	str = strtok(str, "\n");

	int size = start; 
	char *ptr = strtok(str, delimiter.c_str());
	int flagged = 1;
	if(ptr==NULL)
	flagged = 0;
	while (flagged){
		words[size++] = ptr;
		ptr = strtok(NULL, delimiter.c_str());
		if(ptr==NULL)
		flagged = 0;
	}

	return size;
}

int literals_Output(int locctr, FILE *outputFile, symtab *base){
	while (true){
	int flagged = 0;
	if(literal_pool.size()==0)
	flagged = 1;
        if(flagged){
            break;
        }
		string lit = *literal_pool.begin();

        FILE *filesymtab = fopen(symtabFile, "a");
        char addr[MAXW];
        sprintf(addr, "%0X", locctr);
        base->table[lit] = {addr};
        fprintf(filesymtab, "%s\t%s\n", addr, lit.c_str());
        fclose(filesymtab);

        fprintf(outputFile, "%04X\t*\t%s\n", locctr, lit.c_str());
		if(lit[1] != 'C')
		flagged = 1;
		if(flagged){
            locctr += (lit.length() - 4 + 1) / 2;
		}else{
			locctr += lit.length() - 4;
		}
        
        for(auto it:literal_pool){
            literal_pool.erase(it);
            break;
        }
		
	}
	return locctr;
}

int hExToSinGleChar(char c){
    int n;
    int flagged = 0;
    if ((c >= '0') && (c <= '9'))
    {
    	flagged = 1;
    }
    if (f(flagged,0)){
        n = c - 'A' + 10;
    }else{
        n = c - '0';
    }
    return n;
}

int hExToInt(string s){
    reverse(s.begin(), s.end());
    int j = 1;
    int ans = 0;
    int i = 0;
    while(i < s.length()){
        ans += (hExToSinGleChar(s[i]) * j);
        j = j * 16;
        i++;
    }
    return ans;
}

FILE *progamFile, *intermediateFile, *objectFile, *listFile;

string OPCODE, LABEL="", OPERAND;
int LOCCTR;
bool extended;

int incr_LOCCTR(){
    if (op_code *info = search_optab(OPCODE)){
        if (extended){
            LOCCTR = LOCCTR + (4 * ILEN);
        }else{
            LOCCTR = LOCCTR + (info->format * ILEN);
        }
    }else if (f1(OPCODE,"WORD")){
        LOCCTR = LOCCTR + 3;
    }else if (f1(OPCODE,"RESW")){
        LOCCTR = LOCCTR + (3 * stoi(OPERAND));
    }else if (f1(OPCODE,"RESB")){
        LOCCTR = LOCCTR + stoi(OPERAND);
    }else if (f1(OPCODE,"BYTE")){
        if (f2(OPERAND[0],'C')){
            LOCCTR = LOCCTR + ((OPERAND.length())-3);
        }else if (f2(OPERAND[0],'X')){
            LOCCTR = LOCCTR + (((OPERAND.length())-3+1)/2);
        }
    }else{
        return 1;
    }
    return 0;
}

char* string_Copy(char* destination, const char* source){
    if (destination == NULL)
        return NULL;
 
    char *ptr = destination;
 
    while (*source != '\0'){
        *destination = *source;
        destination++;
        source++;
    }
 
    *destination = '\0';
 
    return ptr;
}

