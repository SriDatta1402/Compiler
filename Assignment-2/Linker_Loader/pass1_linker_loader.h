#include <bits/stdc++.h>
#include "functions_linker_loader.h"
using namespace std;

//pass 1 of the assembler
int pass1_linker_loader(){
	
	input_file.open(inputFileName);

	//this file will contain the external symbol table as contructed after pass1_linker_loader;
	pass1File.open("esTableFile.txt");

	pass1File<<"ControlSec\tSymbolName   Addr    \t Length\n";

	//assign starting address for relocation
	controlSecAddr=progAddr; 

    //stores the name of the current control section
	string current_secname=""; 
	int cs_length=zero;

	string line="";

	while(getline(input_file,line,'\n')){

		vector<string> line_w=readLineToWords(line);

		if(f2(line_w[zero][zero],'H')){

			string csLen="";
			string controlSecAddr_obj="";
			int i = zero;
			while(6>i){
				controlSecAddr_obj+=line_w[1][i];
				i+=1;
			}
			i = 6;
			while(line_w[1].length()>i){
				csLen += line_w[1][i];
				i+=1;
			}

			string temp;
			temp = "";
			i = 1;
			while(line_w[zero].length()>i){
				temp+= line_w[zero][i];
				i+=1;
			}
			line_w[zero] = temp;

			if(find_Ind_By_SymB_Nam(EStable,line_w[zero])>=zero){
				cout<<"ERROR : DUPLICATE SECTION FOUND\n";
				return -1;
			}else{
				current_secname=line_w[zero];
				int addR = hExToInT(controlSecAddr_obj);
				addR += controlSecAddr;
				es new_es (addR,hExToInT(csLen));
				EStable.push_back({line_w[zero],new_es});

				pass1File<<line_w[zero]<<" \t\t  \t\t\t ";
                pass1File << iNtToHeX(new_es.address,4) <<" \t\t ";
                pass1File<< iNtToHeX(new_es.length,4)<<" \t\t \n";
			}

			while(getline(input_file,line,'\n')){
				vector<string> records=readLineToWords(line);

                //if end record
				if(f2(records[zero][zero],'E')){ 
					break;
				}

                //if define records
				if(f2(records[zero][zero],'D')){ 
					DTypeRecords(records);
				}
			}

			controlSecAddr += hExToInT(csLen);
		}

	}
	input_file.close();
	pass1File.close();

	return 1;
}

