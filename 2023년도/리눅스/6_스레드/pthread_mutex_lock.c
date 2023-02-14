#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define PRINT_ERROR(_MSG, _VAL) \
{fprintf(stderr, "Create %s Error : %d",_MSG, _VAL); exit(1);}
void *thread1(void* arg);
void *thread2 (void* arg);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /*mutex 초기화 : 동기화*/
int shared_value;
int main(void)
{
    pthread_t t1,t2;
    int status;
    shared_value =0;
    if (pthread_create(&t1, NULL, thread1, NULL) !=0)
        PRINT_ERROR("Thread", 1);
    
    sleep(1);
    if(pthread_create(&t2, NULL, thread2, NULL) !=0)
        PRINT_ERROR("Thread", 2);
    
    if(pthread_join(t1, (void*)&status) != 0)
        PRINT_ERROR("join()", 1);
    
    if(pthread_join(t2, (void*)&status) != 0)
        PRINT_ERROR("join()", 2);
    status = pthread_mutex_destroy(&mutex); /*꼭 해제를 해라!*/
    printf("Code = %d\n", status);
    printf("Programming is end\n");
    exit(0);
}  
void *thread1(void* arg)
{
    int i;
    
    for (i=0 ; i<10 ; i++)
    {   
        pthread_mutex_lock(&mutex); /* 이걸 for문 밖으로 빼버리면 t1이 끝날때까지 t2가 움직이지 않음*/
        printf("loop1 : %d\n", shared_value++);
        if (i == 10)
            return NULL;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    
    return NULL;
}
void *thread2 (void* arg)
{
    int i;
    for (i=0 ; i<10 ; i++)
    {
        pthread_mutex_lock(&mutex);
        printf("loop2 : %d\n", shared_value++);
        if (i == 10)
            return NULL;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}
