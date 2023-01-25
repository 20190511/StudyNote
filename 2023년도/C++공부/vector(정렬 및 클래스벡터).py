// 2차원 벡터 초기화
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef struct {
    int x;
    int y;
    string name;
}pos;

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

void print2(vector<pos> &v)
{
    for (auto &vs : v)
    {
        cout<<vs.x<<" "<<vs.y<<" "<<vs.name<<endl;
    }
}

bool compare(pos a,pos b)
{
    if (a.x == b.x)
    {
        return a.y < b.y;
    }
    else
    {
        return a.x < b.x;
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
    
    vector<pos> v;
    v.push_back({5,6,"As"});
    v.push_back({3,4,"BA"});
    v.push_back({3,7,"ss"});
    print2(v);
    cout<<endl;
    //정렬 -> compare 함수 정의
    sort(v.begin(), v.end(),compare);
    print2(v);
}
