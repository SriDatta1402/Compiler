/*
ENVIRONMENT THAT HAS BEEN USED : LINUX g++ 
Code Written by : Adduri Sai Sri Datta
Command That has been used in compiling the source code : g++ 190101003_Assign01.cpp
Command That has been used to execute : ./a.out
It generates object.txt file and we can view required output there
*/
#include <bits/stdc++.h>
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
using namespace std;
void solve()
{
   pass1();
   pass2();
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
