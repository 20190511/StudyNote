//https://blockdmask.tistory.com/73
#include <iostream>
#include <deque>
using namespace std;

void print(const deque<int> &q)
{
    for (auto iter = q.begin() ; iter != q.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
}


int main (void)
{
    deque<int> q;
    q.push_back(1);
    q.push_back(2);
    q.push_back(3);
    for (deque<int>::iterator it = q.begin() ; it != q.end() ; it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    for (int i = 0 ; i < q.size() ; i++)
    {
        cout<<q[i]<<" ";
    }
    cout<<endl;
    return 1;
}
