/*
ENVIRONMENT: LINUX g++
gcc version : 9.3.0
Command To compile the source code : g++ 190101003_Assign2_linker_loader.cpp
Command to execute : ./a.out
*/
#include <bits/stdc++.h>
#include "pass2_assembler.h"
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
void solve()
{
    load_OPTAB();

    FILE *st = fopen(symtabFile, "w");
    fclose(st);

    progamFile = fopen("program.txt", "r");
    intermediateFile = fopen("intermediate.txt", "w");
    objectFile = fopen("objectCodeFile.txt", "w");
    listFile = fopen("codeListingFile.txt", "w");

    pass1_assembler();

    fclose(intermediateFile);

    intermediateFile = fopen("intermediate.txt", "r");

    pass2_assembler();

    fclose(progamFile);
    fclose(intermediateFile);
    fclose(objectFile);
}
int main ()
{
    ios_base::sync_with_stdio(0) ;
    cin.tie(0) ; cout.tie(0) ;
 
    ll t = 1;
    //cin >> t ;
    while ( t-- )
    {
        solve() ;
    }
    return 0 ;
}
