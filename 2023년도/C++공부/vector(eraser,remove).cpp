#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// algorithm 의 remove 와 erase 의 장단점
/*
 1. remove() size가 줄어들지 않고, 마지막 값을 마지막에 덧붙여줌
 2. erase() 사이즈가 줄어들고 return값이 의미가 없음.
    
그래서 둘의 장점을 조합하여 v.erase(remove(v.begin(), v.end(), 지우려는값), v.end()) 
와 같이 사용할 수 있다.

*/

void print(vector<int> vs)
{
    for (int i = 0 ; i < vs.size() ; i++)
    {
        cout<<vs[i]<<" ";
    }
    cout<<endl;
}


int main(void)
{
    vector<int> v{1,2,3,4,5};
    //remove(v.begin(), v.end(), 1);
    v.erase(remove(v.begin(), v.end(), 1), v.end());
    print(v);
    
    return 1;
}
