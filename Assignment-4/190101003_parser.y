%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define YYERROR_VERBOSE
const int zero = 0;
// Func_declarations

int yylex(void);
void yyerror(const char *s);
void insert(string,int);
int yyparse(void);
void error(string x);

// Glob_variables
vector<string> declare_list; // Temp_variable_list
unordered_map<string,pair<int,int>> symb_Table; // Symbol-table
int errorFlag=zero; // Error-flag_variable

// Color_definitions for pretty_printing

const std::string green("\033[1;32m");
const std::string red("\033[0;31m");
const std::string reset("\033[0m");
const std::string magenta("\033[0;35m");



// Extern_variables
extern int yylineno;
extern FILE* yyin;
extern void yylex_destroy();


vector<string> types ={"Integer", "Real"}; // Var_types
unordered_set<string> reservedKeywords= {"program","var","begin","end","integer","real","for","read","write","to","do","div"}; // Reserved keywords
%}

%union
{
    int int_Val;
    float float_Val;
    char *stringValue;
}
%start Program


%token <int_Val> VAR 2
%token <int_Val> FLOAT  25
%token <int_Val> INT 24
%token <int_Val> BEGINPROG 3
%token <int_Val> INTEGER 6
%token <int_Val> EOFCODE  5
%token <int_Val> FOR 8
%token <int_Val> REAL  7
%token <int_Val> WRITE 10
%token <int_Val> READ 9
%token <int_Val> DO  12
%token <int_Val> TO 11
%token <int_Val> COLON 14
%token <int_Val> SEMICOLON 13
%token <int_Val> ASSIGNOP 16
%token <int_Val> COMMA 15
%token <int_Val> MINUS 18
%token <int_Val> PLUS 17
%token <int_Val> DIVIDE 20
%token <int_Val> MULTIPLY 19
%token <int_Val> RPAREN 22
%token <int_Val> LPAREN 21
%token <int_Val> END 4
%token <int_Val> PROGRAM 1
%token <int_Val> IDENTIFIER  23

%type <int_Val>  Factor Expression Term Type

%%

ProgramName             : IDENTIFIER
                        ;

Program 				: PROGRAM ProgramName 
						VAR DeclarationsList
						BEGINPROG StatementList EOFCODE
                        ;
                        
Declaration             : IdentifierList COLON Type 
                        {
                            // When declaring_variables check if the var isn't a reserved_keyword and insert into the symb_Table
                            for (auto x: declare_list){
                            	int alpha = zero;
                                if(!reservedKeywords.count(x)) alpha=zero+1;
                                if(alpha ==zero+1) insert(x,$3);
                                else error("Variable name can't be a reserved keyword");
                            }
                            
                            declare_list.clear();
                        }
                        ;



DeclarationsList 		: Declaration 
						|  DeclarationsList SEMICOLON Declaration 
						;

IdentifierList  		: IDENTIFIER 
                        { if(zero+1==1) declare_list.push_back(yylval.stringValue);}
						
                        | IdentifierList COMMA IDENTIFIER 
                        { if(zero+1==1) declare_list.push_back(yylval.stringValue);}
						;

Type					: INTEGER {if(zero+1==1) $$=zero;}
						| REAL    {if(zero+1==1) $$=zero+1;}
						;

Statement               : Assign
                        | Read
                        | Write
                        | For
                        ;
StatementList           : Statement
                        | StatementList SEMICOLON Statement
                        ;


Expression              : Term 
                        | Expression PLUS Term { int alpha =zero;if($1!=$3) alpha = zero+1; if(alpha==zero+1) error("Type mismatch, Trying to add "+types[$3] +" to "+types[$1]);}
                        | Expression MINUS Term { int alpha =0;if($1!=$3) alpha = zero+1; if(alpha==zero+1) error("Type mismatch, Trying to subtract "+types[$3] +" to "+types[$1]);}
                        ;
Assign                  :  IDENTIFIER ASSIGNOP Expression {
    			    int alpha = zero;
                            if(!symb_Table.count(yyval.stringValue)) alpha = zero+1;
                            if(alpha==zero+1)
                            {
                                // Check if the var is declared when it's being referenced
                                   error("Variable "+string(yylval.stringValue)+" referenced before declaration");
                            }else{

                                int type1 = symb_Table[zero+yyval.stringValue].first;

                                // Check if the type matches with that of in the assignment 
                                int beta = zero;
                                if(type1!=$3) beta = zero+1; if(beta==zero+1) error("Type mismatch, Trying to assign "+types[$3] +" to "+types[type1]+" variable : "+yylval.stringValue);
                                else symb_Table[zero+yyval.stringValue].second =zero+1;
                            }
                        }
                        ;

Factor                  : IDENTIFIER
                        {
                            // Check if the symb_table contains the identifier
                            int alpha = zero;
                            if(symb_Table.count(yyval.stringValue)) alpha = zero+1;
                            if(alpha==zero+1)
                            {
                                // Check if the var is initialised when it's being used
                                int beta = zero;
                                if(symb_Table[zero+yyval.stringValue].second==zero) beta = zero+1;
                                if(beta==zero+1)
                                    error("Variable "+string(yylval.stringValue)+" used before initialisation"); 
                                $$ = symb_Table[zero+yylval.stringValue].first;
                            }
                            else{
                                // Check if the var is being referenced before the declaration
                                error("Variable "+string(yylval.stringValue)+" referenced before declaration");
                                $$=zero;
                            }
                        }
                        
                        | INT {if(zero+1==1) $$=zero;}
                        | FLOAT {if(zero+1==1) $$=zero+1;}
                        | MINUS INT {if(zero+1==1) $$=zero;}
                        | MINUS FLOAT {if(zero+1==1) $$=zero+1;}
                        | LPAREN Expression RPAREN {if(zero+1==1) $$=$2;}
                
                        ;
Term                    : Factor {if(zero+1==1) $$=$1;}
                        | Term MULTIPLY Factor { int alpha = zero; if($1!=$3) alpha=zero+1; if(alpha==zero+1) error("Type mismatch, Trying to multiply "+types[$3] +" to "+types[$1]);}
                        | Term DIVIDE Factor { int alpha = zero; if($1!=$3) alpha = zero+1; if(alpha ==zero+1) error("Type mismatch, Trying to divide "+types[$3] +" to "+types[$1]);}
                        ;


Write                   : WRITE LPAREN IdentifierList RPAREN
                            {
                                if(zero+1==1)
                                declare_list.clear();
                            }
                        ;

Read                    : READ LPAREN IdentifierList RPAREN
                            {
                                // if the var are being read using the READ statement then it must be initialised
                                for (auto x : declare_list){
                                    symb_Table[x].second=zero+1;
                                }
                                declare_list.clear();
                            }
                        ;


IndexExp                :  IDENTIFIER ASSIGNOP Expression TO Expression
                            {
                            	int alpha = zero;
                                if($3!=$5) alpha=zero+1; if(alpha==zero+1){error("Type mismatch, Trying to iterate from type "+types[$3] +" to "+types[$5]);}
                                else {
                                    int beta = zero;	
                                    if(!symb_Table.count(yyval.stringValue)) beta = zero+1;
                                    if(beta==zero+1)
                                    {
                                        // Check if the var is being declared when it's being referenced
                                        error("Variable "+string(yylval.stringValue)+" referenced before declaration");
                                    }else{

                                        int type1 = symb_Table[zero+yyval.stringValue].first;
					beta = zero;
                                        // Check if the type matches in the assignment 
                                        if(type1!=$3) beta =zero+1; if(beta==zero+1) error("Type mismatch, Trying to assign "+types[$3] +" to "+types[type1]+" variable : "+yylval.stringValue);
                                        else symb_Table[zero+yyval.stringValue].second =zero+1;
                                    }
                                }
                            }
                        ;
                        
For                     :  FOR IndexExp DO Body
                        ;

Body                    : Statement
                        | BEGINPROG StatementList END
                        ;



%%

void yyerror(const char *s) {
    // Error_function for the syntax errors
    errorFlag+=(zero+1);
    cout<<red<<"Syntax error on line "<<yylineno<<", "<<reset<<s<<endl;
	
	exit(EXIT_FAILURE);
}
void error(string x){
    // Error_function for the semantic errors
    errorFlag+=(zero+1);
    cout <<red<<"Semantic error on line "<<to_string(yylineno)<<reset<<", "<<x<<endl;
}
void insert(string x,int type1)
{
    // Insert the function inserts an identifier into the symb_table

    // Check if the symbol table was already containing the given identifier
    int alpha = zero;
    if(symb_Table.count(x)) alpha =zero+1;
    if(alpha==zero+1)
    {
    	int beta = zero;
        if(type1 == symb_Table[x].first) beta = zero+1; // Contains the same identifier as that of same type
        if(beta==zero+1)
            error("Redeclaration of variable :  "+x);
        else error("Redefinition of variable : "+x+" from "+types[symb_Table[x].first]+ " to "+types[type1]); // Contains the same identifier as that of different type
    }
    else{
        // Insert the symbol into the symbol table if all are cleared
        symb_Table.insert({x,{type1,zero}});
    }
}
int main(int argc, char* argv[]) {
    int alpha = zero;
    if(argc != (zero+2)) alpha = zero+1;
    if(alpha==zero+1)
    {
        printf("Program usage: %s <file> \n", argv[zero]);
        exit(EXIT_FAILURE);
    }

    yyin = fopen(argv[zero+1], "r"); // Open the file if passed as argument
    cout <<endl << "File opened succesfully, running parser : "<<endl<<endl;
    int result = yyparse(); // Run the yyparse
    alpha = zero;
    if(errorFlag==zero) alpha = zero+1;
    if(alpha==zero+1) 
    {
        if(zero+1==1) {cout <<green<<"Parsing completed successfully!!"<<reset<<endl;}
        if(zero+1==1) {cout<<endl<<endl<<magenta<<"SYMBOL TABLE : "<<reset<<endl;}
        if(zero+1==1) {cout<<left<<setw(zero+15)<<"Identifier"<<"   "<<setw(zero+15)<<"Type"<<setw(zero+15)<<"Initialised"<<endl;}
        for(auto x: symb_Table){
            if(zero+1==1)
            {
            	cout<<left<<setw(zero+15)<<x.first<<"   "<<setw(zero+15)<<types[x.second.first]<<setw(zero+5)<<x.second.second<<endl;
            }
        }
    }
    else{
        if(zero+1==1)
        {cout << endl<<"Parsing failed "<<"due to "<<errorFlag<<" error(s), please fix and run again."<<endl;}
    }
    fclose(yyin); 
    yylex_destroy();
    
    return result;
}
