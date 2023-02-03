/*
    1. void* 포인터 : 모든 타입의 주솟값을 받을 수 있는 포인터 (함수 주솟값도 받을 수 있다.)
        -> 일단 주솟값만 받아오고 타입(형태)는 나중에 적용한다. 대표적인게 malloc
        

    2. malloc, calloc, realloc : 결론, (int*) 와 같이 형변환을 안해줘도 돌아는 간다.!
    (안 해도 돌아가지만 차후에 코드가 꼬일 수 있으므로 형변환을 할 것.)
    // A. memory allocate
    void* malloc(사이즈);
        ex) int *ptr = (int*)malloc(sizeof(int) * 5);
    
    // B. count allocate -> 모든 블럭을 0으로 만든다.
    void* calloc(블럭개수, 블럭사이즈);
        ex) int *ptr = (int*)calloc(5, sizeof(int));
    
    // C. ReAllocate -> 메모리 재할당 확장되면 뒷공간으로 0으로 됨
    void* realloc(*ptr, 재할당 사이즈);
        ex) ptr = realloc(ptr, sizeof(int)*10);

    
*/

#include <stdio.h>
#include <stdlib.h>
#define ARR_SIZE    10

int main(void)
{
    //int *arr = malloc(sizeof(int)*ARR_SIZE); //해당 코드 돌아가지만 비추.
    int *arr = (int*)malloc(sizeof(int)*ARR_SIZE);

    if (arr==NULL)
    {
        printf("NULL PTR");
        return 1;
    }
        
    for (int i = 1 ; i < ARR_SIZE+1 ; i++)
        arr[i-1] = i*i;
        
    for (int i = 0 ; i < ARR_SIZE ; i++)
        printf("%d ",arr[i]);
    printf("\n");
    
    arr = realloc(arr, sizeof(int) * ARR_SIZE * 2);
    

    for (int i = 0 ; i < ARR_SIZE*2 ; i++)
        printf("%d ",arr[i]);
    printf("\n");
    return 0;
}
