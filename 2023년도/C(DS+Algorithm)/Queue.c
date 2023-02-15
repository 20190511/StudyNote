
#include <stdio.h>
#include <stdlib.h>
#define DEBUG
typedef struct Node
{
    int data;
    struct Node *next;
}Node;
typedef struct Queue
{
    Node *front;
    Node *rear;
    int count;
}Queue;
void initQueue (Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
}
int isEmpty(Queue *queue)
{
    return queue->count == 0; /* 큐 노드가 0이면 빈 상태*/
}
void enqueue (Queue *queue, int data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(queue))
    {
        queue->front = newNode;
        queue->rear = newNode;
        queue->count++;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
        queue->count++;
    }
}
int popback(Queue *queue)
{
    Node *delNode; /*삭제할 노드*/
    int return_val;
    if (isEmpty(queue))
    {
        printf("NULL QUEUE ! \n");
        return -1;
    }
    else if (queue->count==1)
    {
        delNode = queue->front;
        return_val = delNode->data;
        queue->front = queue->rear = NULL;
        queue->count--;
        free(delNode);
        return return_val;

    }
    else
    {
        Node *delNode = queue->front;
        return_val = delNode->data;
        queue->front = queue->front->next;
        delNode->next = NULL;
        free(delNode);
        queue->count--;
        return return_val;
    }
}

void print_node(Queue *queue)
{
    int i;
    Node *original = queue->front;
    printf("Start print queue\n");
    printf("[front] ");
    for (i=0 ; i<queue->count ; i++)
    {
        printf("%d", queue->front->data);
        queue->front = queue->front->next;
        if (!queue->front)
            printf(" [rear]\n");
        else
            printf("->");
    }
    queue->front = original;
    printf("end print queue\n");
}
#ifdef DEBUG
int main(void)
{
    Queue q;
    initQueue(&q);
    enqueue(&q, 5);
    enqueue(&q, 1);
    enqueue(&q, 8);
    enqueue(&q, 3);
    print_node(&q);
    int result = popback(&q);
    print_node(&q);
    enqueue(&q, 10);
    enqueue(&q, 51);
    enqueue(&q, 33);
    print_node(&q);
    
    printf("pop value is %d\n", result);
    return 0;
}
#endif
