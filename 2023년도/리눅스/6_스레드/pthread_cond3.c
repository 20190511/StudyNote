
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define VALUE_DONE 10
#define VALUE_START 4
#define VALUE_END 7
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void *thread1(void* arg);
void *thread2(void* arg);

int glo_val = 0;
int main(void)
{
    pthread_t tid1 ,tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    printf("final value = %d\n", glo_val);
    exit(0);
}
void *thread1(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond, &lock);
        glo_val++;
        printf("thread1, global value = %d\n", glo_val);
        pthread_mutex_unlock(&lock);
        if (glo_val >= VALUE_DONE)
            return NULL;
    }
}
void *thread2(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock); /* mutex lock로 잠근 후 cond 를 사용해야한다!!!*/
        if (glo_val<VALUE_START || glo_val>VALUE_END)
        {
            pthread_cond_signal(&cond); /*해당 변수 범위 밖이면 thread1 시그널보냄*/
        }
        else
        {
            /*1번 스레드가 변경중에 있으면 glo_val 접근 불가.*/
            glo_val++;
            printf("thread2, global val = %d\n", glo_val);
        }
        pthread_mutex_unlock(&lock);
        if (glo_val >= VALUE_DONE)
            return NULL;
    }
}
/*
thread1, global value = 1
thread1, global value = 2
thread1, global value = 3
thread1, global value = 4
thread2, global val = 5
thread2, global val = 6
thread2, global val = 7
thread2, global val = 8
thread1, global value = 9
thread1, global value = 10
final value = 10
*/
