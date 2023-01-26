#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;
void bfs(vector<vector<int>> &graph, int start, vector<bool> &visited)
{
    visited[start] = true;
    deque<int> queue;
    queue.push_back(start);
    
    while (queue.size() != 0)
    {
        int next = queue.front();
        queue.pop_front();
        cout<<next<<" ";
        for (int i = 0 ; i < graph[next].size() ; i++)
        {
            int to = graph[next][i];
            if (visited[to] == false)
            {
                visited[to] = true;
                queue.push_back(to);
            }
        }
    }
    cout<<endl;
}


void make_graph(vector<vector<int>> &graph)
{
    graph[1].push_back(2);
    graph[1].push_back(3);
    graph[1].push_back(8);
    
    graph[2].push_back(1);
    graph[2].push_back(7);
    
    graph[3].push_back(1);
    graph[3].push_back(4);
    graph[3].push_back(5);
    
    graph[4].push_back(3);
    graph[4].push_back(5);
    
    graph[5].push_back(3);
    graph[5].push_back(3);
    
    graph[6].push_back(7);
    
    graph[7].push_back(2);
    graph[7].push_back(6);
    graph[7].push_back(8);
    
    graph[8].push_back(1);
    graph[8].push_back(7);
}

int main(void)
{
    int n = 8;
    vector<vector<int>> graph(n+1);
    vector<bool> visited(n+1,false);
    visited[0] = true;
    make_graph(graph);
    bfs(graph, 1 , visited);
    
    return 1;
}

//bfs


