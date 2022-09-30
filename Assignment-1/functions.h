#include <bits/stdc++.h>
using namespace std;
//length of program in hex
string PROG_LEN;

//max length of text_record
const int TEXT_RECORD_LEN = 69;

//Op_Code_Tab
const unordered_map<string, string> OPTAB = {
    {"LDA", "00"},
    {"LDX", "04"},
    {"LDL", "08"},
    {"STA", "0C"},
    {"STX", "10"},
    {"STL", "14"},
    {"LDCH", "50"},
    {"STCH", "54"},
    {"ADD", "18"},
    {"SUB", "1C"},
    {"MUL", "20"},
    {"DIV", "24"},
    {"COMP", "28"},
    {"J", "3C"},
    {"JLT", "38"},
    {"JEQ", "30"},
    {"JGT", "34"},
    {"JSUB", "48"},
    {"RSUB", "4C"},
    {"TIX", "2C"},
    {"TD", "E0"},
    {"RD", "D8"},
    {"WD", "DC"}};

//Sym_Tab
unordered_map<string, string> SYMTAB;
//variables
    string LOCCTR;
    string START_ADDR;

//LAB, OP_CODE, OPERAND
string LABEL;
string OPCODE;
string OPERAND;

//func to conv int from 0 to 15 to a single char hex
char conv_to_HexChar_from_int(int n)
{
    char c;
    int Flagged = 0;
    if(n>9)
    Flagged = 1;
    if (Flagged==1)
    {
        c = 'A' + (n - 10);
      
    }
    else
    {
          c = '0' + n;
    }
    return c;
}

//conv single char hex to int
int conv_to_Int_from_HexChar(char c)
{
    int n;
    int Flagged = 0;
    if((c <= '9') && (c >= '0'))
    Flagged = 1;
    if (Flagged==0)
    {
        n = c - 'A' + 10;
    }
    else
    {
           n = c - '0';
    }
    return n;
}

//conv int val to hex string of len 6
string convert_Int_To_Hex(int n)
{
    string s = "";
    int Flagged = 1;
    while (Flagged)
    {
        s = conv_to_HexChar_from_int(n % 16) + s;
        n = n / 16;
        if(n==0)
        Flagged = 0;
    }
    Flagged = 1;
    while (Flagged)
    {
        s = '0' + s;
        if(s.length()>=6)
	Flagged = 0;     
    }
    return s;
}

//conv int val to hex str of len 2
string convert_Int_To_HexLen2(int n)
{
    string s = "";
    int Flagged;
    if(n==0)
    Flagged = 0;
    else
    Flagged = 1;
    while (Flagged)
    {
        s = conv_to_HexChar_from_int(n % 16) + s;
        n = n / 16;
        if(n==0)
        Flagged = 0;
    }
    if(s.length()<2)
    Flagged = 1;
    while (Flagged)
    {
        s = '0' + s;
        if(s.length()>=2)
        Flagged = 0;
    }
    return s;
}

//conv hex str to int val
int Conv_Hex_String_to_Int(string s)
{
    reverse(s.begin(), s.end());
    int j = 1;
    int ans = 0;
    int pandora = s.length();
     while(pandora--)
    {
        ans += (conv_to_Int_from_HexChar(s[s.length()-pandora-1]) * j);
        j = j * 16;
    }
    return ans;
}

//conv str to int
int Conv_Str_To_Int(string s)
{
    reverse(s.begin(), s.end());
    int j = 1;
    int ans = 0;
    int pandora = s.length();
    while(pandora--)
    {
        ans += ((s[s.length()-pandora-1] - '0') * j);
        j = j * 10;
    }
    return ans;
}

//len of addr occupied by char or hex const when OP_CODE == BYTE
int Leng_Byt(string s)
{
    int len = 0;
    int pandora = s.length();
    int Flagged;
    if (s[0] == 'X')
    Flagged = 1;
    else if(s[0] == 'C')
    Flagged = 2;
    if (Flagged == 1)
    {
        len = (pandora - 3) / 2;
    }
    else if (Flagged ==2)
    {
        len = pandora - 3;
    }
    return len;
}

//will read line from input file and return true if line is a comment otherwise false if line is not a comment
bool R_input_Line(ifstream *fin)
{
    string line;
    getline(*fin, line);
    LABEL = "";
    OPCODE = "";
    OPERAND = "";
    int ptr = 0;

    //check line is a comment or not
    if (line[0] == '.')
    {
        LABEL = line;
        return true;
    }

    //if LABEL is empty or not
    if (line[0] != ' ')
    {
        //fetch LABEL from the line
        while (ptr != line.length())
        {
            if (line[ptr] == ' ')
            {
                break;
            }
            else
            {
                LABEL += line[ptr];
            }
            ptr++;
        }
    }

    while (line[ptr] == ' ')
    {
        ptr++;
    }

    //fetch the opcode from the line
    while (ptr != line.length())
    {
        if (line[ptr] == ' ')
        {
            break;
        }
        else
        {
            OPCODE += line[ptr];
        }
        ptr++;
    }

    while (line[ptr] == ' ')
    {
        ptr++;
    }

    //fetch the operand from the line
    while ( ptr != line.length())
    {
        if (line[ptr] == ' ')
        {
            break;
        }
        else
        {
            OPERAND += line[ptr];
        }
        ptr++;
    }

    return false;
}

//will read line from intermediate file and return true if line is a comment otherwise false if line is not a comment
bool R_Intermediate_li(string *LOCCTR, ifstream *fin)
{
    string line;
    getline(*fin, line);

    *LOCCTR = "";
    LABEL = "";
    OPCODE = "";
    OPERAND = "";
    int ptr = 0;

    //check if the line is a comment or not
    if (line[0] == '.')
    {
        LABEL = line;
        return true;
    }

    if (line[0] != ' ')
    {
        while (ptr != line.length())
        {
            if (line[ptr] == ' ')
            {
                break;
            }
            else
            {
                *LOCCTR += line[ptr];
            }
            ptr++;
        }
    }
    else
    {
        ptr += 4;
    }

    *LOCCTR = convert_Int_To_Hex(Conv_Hex_String_to_Int(*LOCCTR));

    ptr += 2;

    while ( ptr < line.length())
    {
        if (line[ptr] == ' ')
        {
            break;
        }
        else
        {
            LABEL += line[ptr];
        }
        ptr++;
    }

    ptr += (15 - (LABEL.length()));

    while (ptr < line.length())
    {
        if (line[ptr] == ' ')
        {
            break;
        }
        else
        {
            OPCODE += line[ptr];
        }
        ptr++;
    }

    ptr += (8 - (OPCODE.length()));

    while (ptr < line.length())
    {
        if (line[ptr] == ' ' || line[ptr] == ',')
        {
            break;
        }
        else
        {
            OPERAND += line[ptr];
        }
        ptr++;
    }

    return false;
}

//writes a line on Intermediate file
void W_Intermed_li(string LOCCTR, ofstream *fout)
{
    if (OPCODE != "")
    {
        if (LOCCTR != "")
        {
            *fout << LOCCTR.substr(2) << "  " << LABEL;
            int i = 0;
            while ( i < 15 - LABEL.length())
            {
                *fout << " ";
                i++;
            }
            *fout << OPCODE;
            i=0;
            while (i < 8 - OPCODE.length())
            {
                *fout << " ";
                i++;
            }
            *fout << OPERAND << endl;
        }
        else
        {
            *fout << "      " << LABEL;
            int i =0;
            while(i < 15 - LABEL.length())
            {
                *fout << " ";
                i++;
            }
            *fout << OPCODE;
            i=0;
            while(i < 8 - OPCODE.length())
            {
                *fout << " ";
                i++;
            }
            *fout << OPERAND << endl;
        }
    }
    else
    {
        *fout << LOCCTR << endl;
    }
}

//writes a line on object file
void W_Obj_li(string line, ofstream *fout)
{
    *fout << line << endl;
}

//increments our LOCCTR depending on the value of OPCODE
void Increment_LOCCTR(string *LOCCTR)
{
    if (OPTAB.find(OPCODE) != OPTAB.end())
    {
        //if OPCODE is present in OPTAB
        *LOCCTR = convert_Int_To_Hex(3 + Conv_Hex_String_to_Int(*LOCCTR));
    }
    else if (OPCODE == "WORD")
    {
        *LOCCTR = convert_Int_To_Hex(3 + Conv_Hex_String_to_Int(*LOCCTR));
    }
    else if (OPCODE == "RESW")
    {
        *LOCCTR = convert_Int_To_Hex((3 * Conv_Str_To_Int(OPERAND)) + Conv_Hex_String_to_Int(*LOCCTR));
    }
    else if (OPCODE == "RESB")
    {
        *LOCCTR = convert_Int_To_Hex(Conv_Str_To_Int(OPERAND) + Conv_Hex_String_to_Int(*LOCCTR));
    }
    else if (OPCODE == "BYTE")
    {
        *LOCCTR = convert_Int_To_Hex(Leng_Byt(OPERAND) + Conv_Hex_String_to_Int(*LOCCTR));
    }
    else
    {
        cout << "ERROR:INVALID OPCODE" << endl;
        exit(0);
    }
}

//constructs the object code of the instruction from OPCODE and OPERAND ADDRESS
string ASSM_OBJ_COD(string OPERAND_ADDR)
{
    string OBJ_COD = OPTAB.at(OPCODE);
    OBJ_COD += OPERAND_ADDR.substr(2);
    return OBJ_COD;
}

