#include "algorithms.h"

using namespace std;

template class te<int>;
template <typename T> void te<T>::kill(T x)
{
    cout << x << endl;
}

int Max(int a,int b)
{
    return a > b?a:b;
}

int lcsWithRecursion1(const char* s1,int m,const char* s2,int n)
{
    if(m == 0 || n == 0)
        return 0;

    if(s1[m-1] == s2[n-1])
        return 1 + lcsWithRecursion1(s1,m-1,s2,n-1);
    else
        return Max(lcsWithRecursion1(s1,m,s2,n-1),lcsWithRecursion1(s1,m-1,s2,n));
}

int childRecursion(const char* s1,int m,const char* s2,int n,vector<vector<int>>& index)
{
    if(m == 0 || n == 0)
        return 0;

    if(index[m-1][n-1] > 0)
        return index[m-1][n-1];

    if(s1[m-1] == s2[n-1])
        index[m-1][n-1] = 1 + childRecursion(s1,m-1,s2,n-1,index);
    else
        index[m-1][n-1] = Max(childRecursion(s1,m,s2,n-1,index),childRecursion(s1,m-1,s2,n,index));

    return index[m-1][n-1];
}

int lcsWithRecursion2(const char* s1,int m,const char* s2,int n)
{
    vector<vector<int>> index;

    for(int i = 0; i < m; i++)
        index.push_back(vector<int>(n,-1));

    return childRecursion(s1,m,s2,n,index);
}



