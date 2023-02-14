/* 메인 스레드가 먼저 죽어버리면 새로 생성된 스레드들은 종료한다.*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define THREAD_NUM 5
void *threads(void *arg);
int main (void)
{
    pthread_t tid[THREAD_NUM];
    int i2;
    for (i2=0 ; i2 < THREAD_NUM ; i2++)
    {
        printf("In main : Creating Thread %d\n", i2);
        if (pthread_create(&tid[i2], NULL, threads, (void*)&i2) != 0)
        {
            fprintf(stderr, "Creating Thread Error=%d\n",i2);
            exit(1);
        }
    }
    pthread_exit(NULL); //?
    printf("die\n"); /* 주 스레드가 종료하여 실행X*/
    exit(0);
}
void *threads(void *arg)
{
    sleep(0.5); /* 주 스레드가 종료하여 arg값이 이상하게 전달되는 현상 <- sleep()함수 호출 시 문제생기는 듯.*/
    int thread_idx; 
    thread_idx = *((int*)arg); //int형 포인터값으로 변환한 뒤 -> 포인터
    printf("Hello world!, It's me thread #%d!\n", thread_idx);
    pthread_exit(NULL);
}
/*
 *  작업 실행 중: /home/junhyeong/go2/pthread_exit1 
In main : Creating Thread 0
In main : Creating Thread 1
Hello world!, It's me thread #1!
In main : Creating Thread 2
Hello world!, It's me thread #2!
In main : Creating Thread 3
Hello world!, It's me thread #3!
In main : Creating Thread 4
Hello world!, It's me thread #4!
Hello world!, It's me thread #5!
*/
