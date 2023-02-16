#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE     10

/** Dataset{length, s_node, f_node}
 * 자료구조형태의 Heap 구조생성
 * 
*/
typedef struct Dataset {
    int length;
    int s_node;
    int f_node;
}Dataset;

/*MaxHeap : 0 인덱스는 버려둠.*/
typedef struct Heap{
    Dataset **nodes; /*Dataset 객체 배열*/
    int count;
    int heapSize;
}Heap;

void initHeap (Heap *heap)
{
    int i;
    heap->nodes = (Dataset**)malloc(sizeof(Dataset*)*MAXSIZE); /*일단 MAXSIZE로 동적할당*/
    heap->heapSize = MAXSIZE;
    heap->count = 0;
}

/* Call by pointer 를 위해 이중포인터 사용.*/
void swap(Dataset **aset, Dataset **bset)
{
    Dataset *temp = *aset;
    *aset = *bset;
    *bset = temp;
}
/* 마지막에 삽입 -> 부모비교*/
void push (Heap *heap, Dataset *data)
{

    if (heap->count+2 == heap->heapSize)
    {
        heap->heapSize *= 2; /*heapSize를 2배로 재할당*/
        heap->nodes = realloc(heap->nodes, sizeof(Dataset*)*(heap->heapSize)); /*재할당 시 사이즈 바이트 수라는거 명심할것 ㅋㅋ*/
    }

    heap->nodes[++heap->count] = data;
    
    int me = heap->count;
    for (int parent = me/2 ; parent > 0 ; parent = me/2)
    {
        if (data->length > heap->nodes[parent]->length)
        {
            swap(&heap->nodes[me],&heap->nodes[parent]);
            me /= 2;
            continue;   
        }
        break;
    }
}

/*부모노드 중 큰 값과 자식값을 교체*/
Dataset* pop(Heap *heap)
{
    if (heap->count == 0)
    {
        printf("This heap is NULL\n");
        return NULL;
    }
    Dataset *return_val = heap->nodes[1];
    heap->nodes[1] = heap->nodes[heap->count];
    heap->nodes[heap->count--] = NULL;
    if (heap->count == 0)
        return return_val;
    int root = 1;
    int root_value = heap->nodes[1]->length;
    for (int child = root*2 ; child <= heap->count ; child = root*2)
    {
        if (heap->nodes[child+1] && heap->nodes[child]->length < heap->nodes[child+1]->length)
            child += 1;
        if (heap->nodes[child]->length > root_value)
        {
            swap(&heap->nodes[child], &heap->nodes[root]);
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
        printf("%d ", heap->nodes[i]->length);
        if(line == i+1)
        {
            line *= 2;
            printf("\n");
        }
    }
    printf("\n");
}

Dataset* setting (int length, int s_node, int f_node)
{
    Dataset *newData = (Dataset*)malloc(sizeof(Dataset));
    newData->length = length;
    newData->s_node = s_node;
    newData->f_node = f_node;
    return newData;
}

int main(void)
{
    Heap h;
    initHeap(&h);
    push(&h, setting(2,0,0));
    push(&h, setting(7,0,0));
    push(&h, setting(17,0,0));
    push(&h, setting(3,0,0));
    push(&h, setting(19,0,0));
    push(&h, setting(100,0,0));
    push(&h, setting(36,0,0));
    push(&h, setting(25,0,0));
    push(&h, setting(1,0,0));

    push(&h, setting(5,0,0));
    push(&h, setting(3,0,0));
    push(&h, setting(10,0,0));
    push(&h, setting(600,0,0));
    push(&h, setting(900,0,0));
    push(&h, setting(17,0,0));
    print_node(&h);


    int count = h.count;
    for (int i = 0 ; i < count+1 ; i++)
    {
        printf("%d ", pop(&h)->length);
    }
    printf("\n");

}
