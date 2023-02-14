#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
void *thread(void *arg);
int main(void)
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread, NULL) != NULL)
    {
        fprintf(stderr, "Thread Create Error\n");
        exit(1);
    }
    
    sleep(1);
    printf("쓰레드가 종료되기 전 main 함수가 먼저 종료되면 실행중인 쓰레드 소멸\n");
    printf("메인 종료합니다\n");
    exit(0);
}
void *thread(void *arg)
{
    printf("쓰레드 시작\n");
    sleep(5);
    printf("쓰레드 수행 완료\n");
    pthread_exit(NULL);
    return NULL;
}
/*
쓰레드 시작
쓰레드가 종료되기 전 main 함수가 먼저 종료되면 실행중인 쓰레드 소멸
메인 종료합니다
*/
