#include <bits/stdc++.h>
#include "pass2_linker_loader.h"
using namespace std;
int generate_obj(){
	
	pass2File.open("Output_LinkerFile.txt");

	pass2File<<"Contents Of Memory Address"<<endl;
	
	int addr_counter= progAddr;

	addr_counter-=16;

	int addrEnd =(*Memory_objcode.rbegin()).first;

	int i=zero;
	while(addrEnd>=addr_counter){
		int index=zero;

		if(i%16==zero){
			pass2File<<iNtToHeX(addr_counter,4)<<" \t ";
		}

		if(progAddr>addr_counter){

			pass2File<<'x';
			pass2File<<'x';
			if((i+1)%4==zero){
				pass2File<<"\t";
			}

		}else if((index=find_Ind_By_SymB_Addr(Memory_objcode,addr_counter))>=zero && Memory_objcode[index].second!=""){
			pass2File<<Memory_objcode[index].second;
			if((i+1)%4==zero){
				pass2File<<" \t";	
			}	
		}else{
			pass2File<<'.';
			pass2File<<'.';
			if((i+1)%4==zero){
				pass2File<<"\t";
			}
		}

		if((i+1)%16==zero){
			pass2File<<"\n";
		}

		addr_counter+=1;
		i+=1;
	}

	while((progAddr-addr_counter)%16!=zero){

		pass2File<<'x';
		pass2File<<'x';
		if((i+1)%4==zero){
			pass2File<<"\t";
		}

		i+=1;
		addr_counter+=1;
	}

	pass2File.close();

	return 1;
}


