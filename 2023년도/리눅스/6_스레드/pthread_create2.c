/* 중요한 점 : 스레드의 생성순서와 실행순서는 생각했던것과 다를 수 있다!*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define THREAD_NUM  7
struct thread_data {
    int thread_index;
    int sum;
    char *message;
};
void *ssu_printhello(void *arg);
struct thread_data thread_data_array[THREAD_NUM]; /*8개 쓰레드 구조체 배열 생성*/
char* messages[THREAD_NUM];
int main(void)
{
    pthread_t tid[THREAD_NUM];
    int sum;
    int i;
    sum = 0;
    messages[0] = "English : Hello World!";
    messages[1] = "French : dafasf erewr!";
    messages[2] = "Spanish : Hello World!";
    messages[3] = "Klingon : Hello Wsaforld!";
    messages[4] = "German : sdsafs a!";
    messages[5] = "Russian : Zdravstvyes df!";
    messages[6] = "Japan : sekai e konnichwa!";
    messages[7] = "Latin : Ob te sa!";
    for (i = 0 ; i < THREAD_NUM ; i++)
    {
        sum = sum+i;
        thread_data_array[i].thread_index = i;
        thread_data_array[i].sum = sum;
        thread_data_array[i].message = messages[i];
        printf("Creat thread %d\n", i);
        /* 스레드 생성*/
        if (pthread_create(&tid[i], NULL, ssu_printhello, (void*)&thread_data_array[i]) <0)
        {
            fprintf(stderr, "pthread create Error\n");
            exit(1);
        }
    }
    sleep(5);
    exit(0);
}
void *ssu_printhello(void *arg)
{
    struct thread_data *data;
    char *message;
    int thread_index;
    int sum;
    sleep(1);
    data = (struct thread_data*)arg;
    thread_index = data->thread_index;
    sum = data->sum;
    message = data->message;
    printf("Thread %d: %s SUM=%d\n",thread_index, message, sum);
}

/*
Creat thread 0
Creat thread 1
Creat thread 2
Creat thread 3
Creat thread 4
Creat thread 5
Creat thread 6
Thread 0: English : Hello World! SUM=0
Thread 1: French : dafasf erewr! SUM=1
Thread 4: German : sdsafs a! SUM=10
Thread 2: Spanish : Hello World! SUM=3
Thread 3: Klingon : Hello Wsaforld! SUM=6
Thread 5: Russian : Zdravstvyes df! SUM=15
Thread 6: Japan : sekai e konnichwa! SUM=21*/
