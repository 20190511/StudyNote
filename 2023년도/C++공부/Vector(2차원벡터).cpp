// 2차원 벡터 초기화
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void print(vector<vector<int>> v)
{
    for (vector<int> vec:v)
    {
        for (int num : vec)
        {
            cout<<num<<" ";
        }
        cout<<endl;
    }
}

int main (void)
{
    vector<vector<int>> arr(10, vector<int>(5,0));
    int value = 1;
    for (int i = 0 ; i < 10 ; i++)
    {
        for (int j = 0 ; j < 5 ; j++)
        {
            arr[i][j] = value++;
        }
    }
    print(arr);
    cout<<endl;
    
    //마지막 줄 할당 지워버림
    arr.pop_back();
    print(arr);
    
    arr[arr.size()-1].pop_back();
    print(arr);
    cout<<arr.size()<<endl;
    
}
