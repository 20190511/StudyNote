#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

typedef struct {
    int x;
    int y;
    string name;
}pos;

void print(vector<pos> &a)
{
    for (auto &num : a)
    {
        cout<<num.x<<" "<<num.y<<" "<<num.name<<" "<<endl;
    }
    
    /*
    //같은 구절.
    for (int i = 0 ; i < s.size() ; i++)
    {
        cout<<s[i].x<<" "<<s[i].y<<" "<<s[i].name<<endl;
    }
    */
}

bool compare ()


int main(void)
{
    vector<pos> s;
    s.push_back({1,3,"A"});
    s.push_back({4,5,"B"});
    s.push_back({5,7,"C"});
    print(s);
    
    
    return 1;
}


