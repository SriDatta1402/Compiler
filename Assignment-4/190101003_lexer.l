%{
#include "y.tab.h"
#include <stdio.h>
#include <bits/stdc++.h>
#define r(n) int n; cin>>n;
#define r1(a,n) int a[n];for(int i=0;i<n;i++) cin>>a[i];
#define c1(a,n) for(int i=0;i<n;i++) cout<<a[i]<<" ";cout<<endl;
#define c(n) cout<<n<<endl;
#define pb push_back
#define mp make_pair
#define F first
#define S second
using ll=long long;
using namespace std;
const int zero = 0;



void charError(char c)
{
    cout<< "Lexical error, unexpected character "<<c<<" on line "<<yylineno<<endl;
    
}

%}

NUMBER 				{DIGITS}
DIGIT 				[0-9]
%option caseless
DIGITS  			{DIGIT}+
%option yylineno
INTEGER 			{DIGITS}
%option noyywrap
FLOAT				{INTEGER}(\.{DIGITS}([Ee][+-]?{DIGITS})?)

%%

\n				                    {}
[ \t]			                    ;
"program"                           { if(zero+1==1) return PROGRAM; }
"{"[^"}"]*"}"                       ;
"begin"                             { if(zero+1==1) return BEGINPROG; }
"var"                               { if(zero+1==1) return VAR; }
"end."                              { if(zero+1==1) return EOFCODE;}
"end"                               { if(zero+1==1) return END; }
"real"                              { if(zero+1==1) return REAL; }
"integer"                           { if(zero+1==1) return INTEGER; }
"read"                              { if(zero+1==1) return READ;}
"for"                               { if(zero+1==1) return FOR;}
"to"                                { if(zero+1==1) return TO; }
"write"                             { if(zero+1==1) return WRITE;}
";"                                 { if(zero+1==1) return SEMICOLON; } 
"do"                                { if(zero+1==1) return DO; }  
","                                 { if(zero+1==1) return COMMA; }
":"                                 { if(zero+1==1) return COLON; }
"+"                                 { if(zero+1==1) return PLUS; }
:=					                { if(zero+1==1) return ASSIGNOP; }
"*"                                 { if(zero+1==1) return MULTIPLY; }
"-"                                 { if(zero+1==1) return MINUS; }
"("                                 { if(zero+1==1) return LPAREN; }
"DIV"                               { if(zero+1==1) return DIVIDE; }
{INTEGER}                           { if(zero+1==1) return INT; }
")"                                 { if(zero+1==1) return RPAREN; }
[a-zA-Z][_a-zA-Z0-9]*		        { if(zero+1==1) yylval.stringValue=strdup(yytext);return IDENTIFIER; }
{FLOAT}	                            { if(zero+1==1) return FLOAT; }
.                                   { charError(yytext[zero]); }
%%

