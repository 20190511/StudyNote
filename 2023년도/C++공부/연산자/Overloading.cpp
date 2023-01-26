#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

int move[4][2] = {{1,0},{-1,0},{0,-1},{0,1}};

typedef struct {
    int x;
    int y;
}xy;

vector<xy> a;

void print(vector<xy> &a)
{
    for (vector<xy>::iterator it = a.begin() ; it != a.end() ; it++)
    {
        cout<<it->x<<" "<<it->y<<endl;
    }
    
}


bool cp (xy a, xy b)
{
    if (a.y == b.y)
    {
        return a.x < b.x;
    }
    else
    {
        return a.y < b.y;
    }
}


ostream& operator<<(ostream& out, const xy &a)
{
    out<<a.x<<" "<<a.y;
    return out;
}


void print2 (deque<xy> &ac)
{
    for (deque<xy>::iterator it = ac.begin() ; it != ac.end() ; it++)
    {
        cout<<*it<<endl;
    }
}

int main (void)
{
    a.push_back({1,6});
    a.push_back({3,4});
    a.push_back({5,2});
    print(a);
    
    sort(a.begin(), a.end(), cp);
    print(a);
    
    
    
    deque<xy> b;
    b.push_back({10,11});
    b.push_back({15,9});
    b.push_back({17,10});
    print2(b);
    
    sort(b.begin(), b.end(), cp);
    print2(b);
    cout<<b.front()<<endl;
    return 0;
}
