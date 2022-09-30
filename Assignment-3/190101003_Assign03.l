/*
	ADDURI SAI SRI DATTA, 190101003
	Assignment-3  
	To run the lexical analyser, type the following the cmd:
		lex 190101003_lex.l 
		g++ lex.yy.c or g++ 190101055_lex.yy.c (same as 190101003_lex.yy.c)
		./a.out
     output.txt is generated on running lexical analyser
*/
%{ 

//Important header files

/*#include <bits/stdc++.h>
#include "pass2.h"
#define r(n) int n; cin>>n;
#define r1(a,n) int a[n];for(int i=0;i<n;i++) cin>>a[i];
#define c1(a,n) for(int i=0;i<n;i++) cout<<a[i]<<" ";cout<<endl;
#define c(n) cout<<n<<endl;
#define pb push_back
#define mp make_pair
#define F first
#define S second
using namespace std;
using ll=long long;
#include <bits/stdc++.h>
using namespace std;*/
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <string>
#include <math.h>
#include <stdbool.h>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <unordered_map>
#include <iomanip>
#include <vector>
using namespace std;

// Token definitions (codes)
#define tVar 2
#define tProgram 1
#define tEnd 4
#define tBegin 3
#define tInteger 6
#define tEndDot 5
#define tRead 8
#define tFor 7
#define tTo 10
#define tWrite 9
#define tSemicolon 12
#define tDo 11
#define tComma 14
#define tColon 13
#define tPlus 16
#define tAssignment 15
#define tMultiply 18
#define tMinus 17
#define tOpenBracket 20
#define tDivision 19
#define tID 22
#define tCloseBracket 21
#define tInt 23

bool install_numb(char *str);
bool install_Id(char *str);
//auxillary functions to add number or Id in hash table

int line_cnt=0; 
int ap = 0;
// to count number of lines

char* Id;
int integer;
//to store yytext in these variables

string yytextString;
// to store char * as string type

unordered_map<string,int> hash_Table; 
// hash table to store symbol table
// int values when int->1 and when Id->0

%}

%option noyywrap

%%

PROGRAM 			{return ap+1;}
VAR				    {return ap+2;}
BEGIN  			    {return ap+3;}
END 				{return ap+4;}
"END."          	{return ap+5;}
INTEGER         	{return ap+6;}
FOR 				{return ap+7;}	
READ 				{return ap+8;}
WRITE 			    {return ap+9;}
TO  				{return ap+10;}
DO 				    {return ap+11;}
";" 				{return ap+12;}
":="  				{return ap+13;}
":" 				{return ap+14;}
"," 				{return ap+15;}
"+" 				{return ap+16;}
"-" 				{return ap+17;}
"*" 				{return ap+18;}
DIV 				{return ap+19;}
"(" 				{return ap+20;}
")"					{return ap+21;}
"\n"				{line_cnt++;}
[A-Z]([A-Z0-9])* 	{
						install_Id(yytext);
                        Id=yytext;
						return ap+22;
					}
[0-9]+				{
						install_numb(yytext);
                        integer=atoi(yytext);
						return ap+23;
					}
"\t"                {}
.					{}

%%

bool install_Id(char *str){
    yytextString=yytext;
    int p = ap;
    if(hash_Table.find(yytextString)==hash_Table.end())
    p = ap+1;
    if(p==ap+1){ //not present in hash table
        hash_Table[yytextString]=ap;
        return true;
    }
    return false; //already present in hash table
}

bool install_numb(char *str){
    yytextString=yytext;
    int p = ap;
    if(hash_Table.find(yytextString)==hash_Table.end())
        p = ap+1;
    if(p==ap+1){ //not present in hash table
        hash_Table[yytextString]=ap+1;
        return true;
    }
    return false; //already present in hash table
}

int main(){
	int s=ap;
	yyin = fopen("input.txt","r"); // open the input file
    yyout = fopen("output.txt","w"); //open the output file
    if(!(yyin&&yyout)){
		return ap;
	}
	fprintf(yyout,"Line\t\tToken\t\tToken Specifier\n");
    int prevLineCount=ap-1;
    //read the input file 
	while(s=yylex()){ 
	//as per the matched token we print in the output file
        //when token code is Id or int
        int p = ap;
        if(s==tInt)
            p = ap+1;
        else if(s==tID)
            p = ap+2;
        if(p==ap+1){
            int q = ap;
            if(prevLineCount!=line_cnt)
               q = ap+1;
            if(q==ap+1){
                fprintf(yyout,"%d\t\t%d\t\t#%d\n",line_cnt+1,s,integer);
            } 
            else{
                fprintf(yyout,"\t\t%d\t\t#%d\n",s,integer);
            }
        }
        else if(p==ap+2){
            int q = ap+0;
            if(prevLineCount!=line_cnt)
               q = ap+1;
            if(q==ap+1){
                fprintf(yyout,"%d\t\t%d\t\t^%s\n",line_cnt+1,s,Id);
            } 
            else{
                fprintf(yyout,"\t\t%d\t\t^%s\n",s,Id);
            }
        }
        //when token is any other value except Id or int
        else{
            int q = ap;
            if(prevLineCount!=line_cnt)
               q = ap+1;
            if(q==ap+1){
                fprintf(yyout,"%d\t\t%d\n",line_cnt+1,s);
            } 
            else{
                fprintf(yyout,"\t\t%d\n",s);
            }
        }
		prevLineCount=line_cnt;
	}
	return ap;
}
