#include "heap_data.h"
#include <stdio.h>
#include <stdlib.h>
#define GRAPH_SIZE  5
int graph[GRAPH_SIZE+1][GRAPH_SIZE+1];


void initgraph()
{
    for(int i = 1 ; i < GRAPH_SIZE+1 ; i++)
        for (int j = 1 ; j < GRAPH_SIZE+1 ; j++)
            graph[i][j] = 99999;
}
int* dijkstra(Heap *heap, int start, int size)
{
    initHeap(heap);
    int *distance = (int*)malloc(sizeof(int)*(size+1));
    int i;
    for(i = 1 ; i <= size ; i++)
        distance[i] = 99999;
    
    distance[start] = 0;

    push(heap, setting(0,start,0));

    while (heap->count > 0)
    {
        Dataset *cur_node = pop(heap);
        int lens = cur_node->length;
        int nd = cur_node->s_node;
        if (distance[nd] < lens)
            continue;
        
        for (i = 1 ; i < size+1 ; i++)
        {
            int sub_lens = graph[nd][i];
            int cost = sub_lens + lens;
            if (cost < distance[i] && i != start)
            {
                distance[i] = cost;
                push(heap, setting(cost, i,0));
            }

        }
        
    }
    return distance;
}

/*push(&h, setting(2,0,0));
*/
void settings()
{
    initgraph();
    for (int i = 1 ; i < GRAPH_SIZE+1 ; i++)
        graph[i][i] = 0;

    graph[1][2] = 2;
    graph[1][3] = 3;
    graph[1][4] = 1;
    graph[1][5] = 10;
    graph[2][4] = 2;
    graph[3][4] = 1;
    graph[3][5] = 1;
    graph[4][5] = 3;
    /*0 2 3 1 4 */
}


void settings2()
{
    initgraph();
    for (int i = 1 ; i < GRAPH_SIZE+1 ; i++)
        graph[i][i] = 0;

    graph[5][1] = 1;
    graph[1][2] = 2;
    graph[1][3] = 3;
    graph[2][3] = 4;
    graph[2][4] = 5;
    graph[3][4] = 6;
    /*0 2 3 7 99999*/
}
int main (void)
{
    settings();
    Heap heap;
    int *distance = dijkstra(&heap, 1, GRAPH_SIZE);

    for (int i = 1 ; i < GRAPH_SIZE+1 ; i++)
        printf("%d ",distance[i]);
    printf("\n");
    

}
