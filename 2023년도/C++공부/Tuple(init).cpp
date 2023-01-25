#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
using namespace std;


void print(const int (*arr)[2], int size1, int size2)
{
    for (int i = 0 ; i < size1 ; i++)
    {
        for (int j = 0 ; j < size2 ; j++)
        {
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    
}


void print2(const vector<tuple<int,int,string>> v)
{
    for (auto &s : v)
    {
        cout<<get<0>(s)<<" "<<get<1>(s)<<" "<<get<2>(s)<<endl;
    }
}
int main(void)
{
    int arr[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    print(arr,4,2);
    vector<tuple<int,int,string>> v;
    v.push_back(make_tuple(1,2,"S"));
    v.push_back(make_tuple(4,2,"A"));
    v.push_back(make_tuple(5,2,"B"));
    reverse(v.begin(), v.end());
    print2(v);
    
    
    return 1;
}
