//https://blockdmask.tistory.com/73
#include <iostream>
#include <deque>
#include <utility>
using namespace std;

void print(const deque<int> &q)
{
    for (auto iter = q.begin() ; iter != q.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
}


void print2 (deque<pair<int,int>> &q)
{
    for (deque<pair<int,int>>::iterator iter = q.begin() ; iter != q.end() ; iter++)
    {
        cout<<iter->first<<" "<<iter->second<<endl;
    }
    cout<<endl;
    
    
}

int main (void)
{
    deque<pair<int,int>> queue;
    queue.push_back(make_pair(1,2));
    queue.push_back(make_pair(3,4));
    queue.push_back(make_pair(5,6));
    //<<queue[1].first<<" "<<queue[1].second<<endl;
    print2(queue);

    
}
