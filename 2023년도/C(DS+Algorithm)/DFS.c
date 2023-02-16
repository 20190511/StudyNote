#include <stdio.h>
#include <stdlib.h>
#define  GRAPH_SIZE     8

void dfs(int** graph, int* visited, int start)
{
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 1 ; i < GRAPH_SIZE+1 ; i++)
    {
        if(graph[start][i] != 999 && !visited[i])
            dfs(graph, visited, i);
    }
}

int **graph_setting(int size)
{
    int **graph = (int**)malloc(sizeof(int*) * (size+1));
    for (int i = 0 ; i < size+1 ; i++)
        graph[i] = (int*)malloc(sizeof(int) * (size+1));
    
    for (int i = 0 ; i < size+1 ; i++)
    {
        for (int j = 0 ; j < size+1 ; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = 999;   
        }
    }
    return graph;
}

void setting2 (int*** graph) 
{
    (*graph)[1][2] = 1;
    (*graph)[1][3] = 1;
    (*graph)[1][8] = 1;
    (*graph)[2][1] = 1;
    (*graph)[2][7] = 1;
    (*graph)[3][1] = 1;
    (*graph)[3][4] = 1;
    (*graph)[3][5] = 1;
    (*graph)[4][3] = 1;
    (*graph)[4][5] = 1;
    (*graph)[5][3] = 1;
    (*graph)[5][4] = 1;
    (*graph)[6][7] = 1;
    (*graph)[7][2] = 1;
    (*graph)[7][6] = 1;
    (*graph)[7][8] = 1;
    (*graph)[8][1] = 1;
    (*graph)[8][7] = 1;
}
int* initVisited()
{
    int *visited = (int*)malloc(sizeof(int) * GRAPH_SIZE+1);
    for (int i = 1 ; i < GRAPH_SIZE+1 ; i++)
        visited[i] = 0;
    visited[0] = 1;
    return visited;
}
int main(void)
{
    int **graph = graph_setting(GRAPH_SIZE);
    setting2(&graph);
    dfs(graph, initVisited(), 1);
    printf("\n");
    return 0;
}
