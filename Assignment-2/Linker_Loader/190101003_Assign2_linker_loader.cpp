/*
ENVIRONMENT: LINUX g++
gcc version : 9.3.0
Command To compile the source code : g++ 190101003_Assign2_linker_loader.cpp
Command to execute : ./a.out
*/
#include <bits/stdc++.h>
#include "generate_obj.h"
#define forinc(i,a,b) for(int i=a;i<=b;++i)
#define fordec(i,a,b) for(int i=a;i>=b;--i)
#define forv(i,x) for(auto &i:x)
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l,int r)
{
    return l+rng()%(r-l+1);
}
#define in ({int x=0;int c=getchar(),n=0;for(;!isdigit(c);c=getchar()) n=(c=='-');for(;isdigit(c);c=getchar()) x=x*10+c-'0';n?-x:x;})
#define pii pair<int,int>
#define fi first
#define se second
#define batbit(i,x) (x|(1ll<<i))
#define getbit(i,x) ((x>>i)&1)
using pp=long long;
using namespace std;
void solve()
{
	inputFileName="linker_input.txt";
	for(int i=0;i<100;i++)
   	{
   	   int j;
   	   j++;
   	} 
	if(!zero){
	progAddr=hExToInT("4000");

	pass1_linker_loader();

	cout<<"es table created in pass1File.txt\n";
}
	if(!zero){
	pass2_linker_loader();

	cout<<"memory table created in pass2File.txt\n";

	generate_obj();
	}
}
int main(){
	pp t = 1;
   	 //cin >> t ;
   	for(int i=0;i<100;i++)
   	{
   	   int j;
   	   j++;
   	} 
    	while ( t-- )
    	{
           solve() ;
    	}
   	 return 0 ;
	
}
