#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE     10

/*MaxHeap : 0 인덱스는 버려둠.*/
typedef struct Heap{
    int *nodes;
    int count;
    int heapSize;
}Heap;

void initHeap (Heap *heap)
{
    int i;
    heap->nodes = (int*)malloc(sizeof(int)*MAXSIZE); /*일단 MAXSIZE로 동적할당*/
    heap->heapSize = MAXSIZE;
    heap->count = 0;
    for (i=0 ; i<MAXSIZE ; i++)
        heap->nodes[i] = 0;
}

/* 마지막에 삽입 -> 부모비교*/
void push (Heap *heap, int data)
{

    if (heap->count+2 == heap->heapSize)
    {
        heap->heapSize *= 2; /*heapSize를 2배로 재할당*/
        void* ptr = heap->nodes;
        heap->nodes = realloc(heap->nodes, sizeof(int)*(heap->heapSize)); /*재할당 시 사이즈 바이트 수라는거 명심할것 ㅋㅋ*/
    }

    heap->nodes[++heap->count] = data;
    
    int me = heap->count;
    for (int parent = me/2 ; parent > 0 ; parent = me/2)
    {
        if (data > heap->nodes[parent])
        {
            heap->nodes[me] = heap->nodes[parent];
            heap->nodes[parent] = data;
            me /= 2;
            continue;   
        }
        break;
    }
}

/*부모노드 중 큰 값과 자식값을 교체*/
int pop(Heap *heap)
{
    if (heap->count == 0)
    {
        printf("This heap is NULL\n");
        return -1;
    }
    int return_val = heap->nodes[1];
    heap->nodes[1] = heap->nodes[heap->count];
    heap->nodes[heap->count--] = 0;
    
    int root = 1;
    int root_value = heap->nodes[1];
    for (int child = root*2 ; child <= heap->count ; child = root*2)
    {
        if (heap->nodes[child] < heap->nodes[child+1])
            child += 1;
        if (heap->nodes[child] > root_value)
        {
            heap->nodes[root] = heap->nodes[child];
            heap->nodes[child] = root_value;
            root = child;   /*해당노드를 자식노드로.*/
            continue;
        }
        else
            break;
    }
    return return_val;
}



void print_node (Heap *heap)
{
    int i;
    int line = 2;
    for (i=1 ; i<=heap->count ; i++)
    {
        printf("%d ", heap->nodes[i]);
        if(line == i+1)
        {
            line *= 2;
            printf("\n");
        }
    }
    printf("\n");
}

int main(void)
{
    Heap h;
    initHeap(&h);
    push(&h, 2);
    push(&h, 7);
    push(&h, 17);
    push(&h, 3);
    push(&h, 19);
    push(&h, 100);
    push(&h, 36);
    push(&h, 25);
    push(&h, 1);
    
    push(&h, 5);
    push(&h, 3);
    push(&h, 10);
    push(&h, 600);
    push(&h, 900);
    push(&h, 17);
    print_node(&h);
    printf("\n");

    int count = h.count;
    for (int i = 0 ; i < count ; i++)
    {
        printf("%d ", pop(&h));
    }
    printf("\n");

}
