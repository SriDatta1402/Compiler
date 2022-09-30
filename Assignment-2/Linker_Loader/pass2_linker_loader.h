#include <bits/stdc++.h>
#include "pass1_linker_loader.h"
using namespace std;
//pass2_linker_loader of the loader 
int pass2_linker_loader(){

	input_file.open(inputFileName);

	int cs_len;

	string line="";

	controlSecAddr=progAddr;

	execAddr=progAddr;

	//reading the input file 
	while(getline(input_file,line,'\n')){

		vector<string> line_w=readLineToWords(line);

		if(f2(line_w[zero][zero],'H')){

			string temp;
			temp = "";
			int i = 1;
			while(line_w[zero].length()>i){
				temp+= line_w[zero][i];
				i+=1;
			}
			line_w[zero] = temp;

			string current_secname;
			current_secname=line_w[zero];
			int index=find_Ind_By_SymB_Nam(EStable,current_secname);
			cs_len=EStable[index].second.length;

			while(getline(input_file,line,'\n')){
				vector<string> records=readLineToWords(line);

                //if end of section
				if(f2(records[zero][zero],'E')){
					break;
				}

                //if text record
				if(f2(records[zero][zero],'T')){ 
					TTypeRecords(records[zero]);

				}

                //if modification record
				if(f2(records[zero][zero],'M')){
					MTypeRecords(records[zero]);
				}
			}

            //if address is specified in the END record 
			if(f2(line[zero],'E')){ 
				if(line.length()>1){
					string temp;
					temp = "";
					i = 1;
					while(line.length()>i){
						temp+= line[i];
						i+=1;
					}
					line = temp;
				
					execAddr = controlSecAddr + hExToInT(line);
				}
			}
			controlSecAddr +=cs_len;
		}

	}

	input_file.close();

    return 1;
}

