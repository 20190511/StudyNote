

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define PRINT_ERR(_MSG,_NUM)\
{fprintf(stderr, "%s : %d ERRROR!\n", _MSG, _NUM); exit(1);}
void* thread1 (void* arg);
void* thread2 (void* arg);
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mute1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mute2 = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
int t1=0 ,t2=0;
int input = 0;
int main(void){
    pthread_t tid1,tid2;
    int status;
    if (pthread_create(&tid1, NULL, thread1, NULL) != 0)
        PRINT_ERR("thread1", 1);
    if (pthread_create(&tid2, NULL, thread2, NULL) != 0)
        PRINT_ERR("thread1", 2);
    
    
    while (1)
    {
        printf("2 이상의 정수 입력: ");
        scanf("%d", &input);
        if (input >= 2)
        {
            pthread_cond_signal(&cond1);
            break;
        }
    }
    pthread_join(tid1, (void*)&status);
    pthread_join(tid2, (void*)&status);
    printf("Finished\n");
    exit(0);    
}

void* thread1 (void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mute1);
        if (input < 2)
            pthread_cond_wait(&cond1, &mute1); /*cond1 <-> cond2 번가라가면서 호출하면서 무한대기*/
        if (input == count)
        {
            pthread_cond_signal(&cond2);
            break;
        }
        if (count == 0)
        {
            t2 += 1;
            count++;
            printf("[%d] : thread1 : %d\n",count, t1);  
        }
        if (count % 2 == 0) /*count가 짝수번째면 thread1 실행*/
        {
            t1 += t2; //dp[n] = dp[n-1]+dp[n-2] 여기서 n-1이 t2, n-2가 바뀌기전 t1이다.
            count++;
            printf("[%d] : thread1 : %d\n",count, t1);    
        }
        pthread_cond_signal(&cond2);
        pthread_cond_wait(&cond1, &mute1);
        pthread_mutex_unlock(&mute1);
    }
    return NULL;
}
void* thread2 (void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mute2);
        if (input < 2)
            pthread_cond_wait(&cond2, &mute2); 
        if (input == count)
        {
            pthread_cond_signal(&cond1);
            break;
        }
        if (count == 1)
        {
            count++;
            printf("[%d] : thread2 : %d\n",count, t2);
        }
        if (count % 2 == 1)
        {
            t2 += t1;
            count++;
            printf("[%d] : thread2 : %d\n",count, t2);
        }
        
        pthread_cond_signal(&cond1);
        pthread_cond_wait(&cond2, &mute2);
        pthread_mutex_unlock(&mute2);
    }
    return NULL;
}
/*
2 이상의 정수 입력: 10
[1] : thread1 : 0
[2] : thread2 : 1
[3] : thread1 : 1
[4] : thread2 : 2
[5] : thread1 : 3
[6] : thread2 : 5
[7] : thread1 : 8
[8] : thread2 : 13
[9] : thread1 : 21
[10] : thread2 : 34
Finished
*/
