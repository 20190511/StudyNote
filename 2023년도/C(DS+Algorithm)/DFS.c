#include <stdio.h>
#include <stdlib.h>
#define  GRAPH_SIZE      8
typedef struct Node {
    struct Node *next;
    int s_node;
    //int f_node;
}Node;

typedef struct Queue
{
    Node *rear;
    Node *front;
    int count;
}Queue;

void initQueue (Queue *queue)
{
    queue->count = 0;
    queue->rear = queue->front = NULL;
}
void append(Queue *queue, Node* newNode)
{
    if (queue->count == 0)
    {
        queue->rear = queue->front = newNode;
        queue->count++;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
        queue->count++;
    }
}

Node pop(Queue *queue)
{
    if (queue->count <= 0)
    {
        printf("this queue is NULL\n");
        return;
    }

    Node* delnode = queue->front; /*원래 있던 노드는 삭제*/
    Node copy = *delnode;
    if (queue->count == 1)
    {   
        queue->front = queue->rear = NULL;
        free(delnode);
        queue->count--;
        return copy;
    }
    else
    {
        queue->front = queue->front->next;
        queue->count--;
        free(delnode);
        return copy;
    }

}

Node *setting (int start)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->s_node = start;
    //newNode->f_node = end;
}


void bfs(int** graph, int start, int size)
{
    int *visited = (int*)malloc(sizeof(int)*(size+1));

    for (int i = 0 ; i < size+1 ; i++)
        visited[i] = 0;
    visited[0] = 1;
    visited[start] = 1;


    Queue q;
    initQueue(&q);
    append(&q, setting(start));
    while (q.count > 0)
    {
        Node next = pop(&q);
        int next_node = next.s_node;
        printf("%d ", next_node);
        
        for (int i = 1 ; i < size+1 ; i++)
        {
            if (graph[next_node][i] != 999 && !visited[i])
            {
                visited[i] = 1;
                append(&q, setting(i));
            }
        }
    
    }
    printf("\n");
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

void setting1 (int** graph) /*Call By Pointer*/
{
    graph[1][2] = 1;
    graph[1][3] = 1;
    graph[1][8] = 1;
    graph[2][1] = 1;
    graph[2][7] = 1;
    graph[3][1] = 1;
    graph[3][4] = 1;
    graph[3][5] = 1;
    graph[4][3] = 1;
    graph[4][5] = 1;
    graph[5][3] = 1;
    graph[5][4] = 1;
    graph[6][7] = 1;
    graph[7][2] = 1;
    graph[7][6] = 1;
    graph[7][8] = 1;
    graph[8][1] = 1;
    graph[8][7] = 1;
}

/** setting2 (&graph); <- graph는 2차원 배열
 * Call By Pointer : 매개변수로 받는 graph 에 값을 복사해서 주는 것이 아니라 
 * graph의 주솟값 그자체를 줘버림으로서 값복사 시간을 줄이는 방식 (어차피 포인터라 차이안나긴함.)
 * */
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
int main(void)
{
    int **graph = graph_setting(GRAPH_SIZE);
    setting2(&graph);
    bfs(graph, 1, GRAPH_SIZE);
    return 0;
}
