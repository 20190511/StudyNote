// 크루스칼 알고리즘 cpp 구현
#include <bits/stdc++.h>
using namespace std;
typedef struct{
    int a;
    int b;
    int c;
}plant;


int find_parent(int *p, int x)
{
    if (x != p[x])
        p[x] = find_parent(p,p[x]);
    return p[x];
}

void unions(int *p, int x, int y)
{
    int a = find_parent(p,x);
    int b = find_parent(p,y);
    
    if (a<b) p[b] = a;
    else p[a] =b;
}

bool cp (plant x, plant y)
{
    if (x.c == y.c)
    {
        return x.a < x.b;
    }
    else
    {
        return x.c < y.c;
    }
}
int main (void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m,k;
    cin>>n>>m>>k;
    int *parent = new int[n+1];
    for (int i = 1 ; i < n+1 ; i++)
    {
        parent[i] = i;
    }
    
    int u;
    for (int i=0 ; i<k ; i++)
    {
        cin>>u;
        unions(parent,u,0);
    }
    
    vector<plant> pl;
    for (int i=0 ; i<m ; i++)
    {
        int a,b,c;
        cin>>a>>b>>c;
        pl.push_back({a,b,c});
    }
    sort(pl.begin(), pl.end(), cp);

    int result = 0;
    for (int i = 0 ; i < m ; i++)
    {
        int a = pl[i].a;
        int b = pl[i].b;
        int c = pl[i].c;
        if (find_parent(parent,a) != find_parent(parent,b))
        {
            result += c;
            unions(parent,a,b);
        }
    }
    cout<<result;
    return 0;
}
