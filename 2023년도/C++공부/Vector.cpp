#include <iostream>
#include <vector>
#include <string>
using namespace std;

void print_vector (const vector<int> vec)
{
    for (auto &v : vec)
        cout<<v<<" ";
    cout<<endl;
}

int main(void)
{
    vector<int> vec(5,1);
    print_vector(vec);
    
    vec.resize(10,2);
    print_vector(vec);
    
    vec.resize(3,3);
    print_vector(vec);
    
    /*
    *
        1 1 1 1 1 
        1 1 1 1 1 2 2 2 2 2 
        1 1 1 
    *
    */
    return 1;
}
