/*
Main Thread tid is 140439909529408, and my pid is 1649
New Thread tid is 140439909525056, and my pid is 1649
PID 는 동일하지만, 쓰레드 번호는 다른 점을 알 수 있다!
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
void *threads (void * arg);
int main(void)
{
    pthread_t tid;
    pid_t pid;
    /* 쓰레드 생성 -> 바로 threads 실행*/
    if (pthread_create(&tid, NULL, threads, NULL) != NULL)
    {
        fprintf(stderr, "make Thread Error\n");
        exit(EXIT_FAILURE);
    }
    /*위의 threads tid가 바뀌지만, 이미 쓰레드는 threads로 가버려서 실행중*/
    tid = pthread_self();
    pid = getpid();
    printf("Main Thread tid is %ld, and my pid is %d\n", tid, pid);
    sleep(2); /*쓰레드 생성되는데 기다려주는 시간*/
    exit(0);
}


void *threads (void * arg)
{
    pthread_t tid;
    pid_t pid;
    pid = getpid();
    tid = pthread_self(); /*자기 자신 tid 반환*/
     
    printf("New Thread tid is %ld, and my pid is %d\n", tid, pid);
    return NULL;
}
