/**
  스레드가 특정시간(5초) 안에 응답이 없을 시 스레드를 종료하는 멀티스레드 테크닉.
  mutex 와 cond_timedwait 를 이용하는 기법.
  
  콘트롤 할 스레드에 pthread_mutex_lock(&lock); <-> pthread_mutex_unlock(&lock); 을 걸어놓고
   pthread_cond_timedwait(&cond, &lock, &timeout) == ETIMEDOUT 를 설정해두고 메인 스레드에서 특정시간 내에
   pthread_cond_broadcast(&cond); 를 호출하지 않을 시 스레드가 강제종료되도록 설정한다.
   
   스레드가 종료될 때 루틴을 추가하거나 pthread_testcancel() 등을 이용하여 
   즉시종료를 할 경우는 아래의 경우에 발생되지 않아 구현하지 않았음 (pthread_cancel 이 즉시 종료하지 않을 수 있는데
    어차피 위의 pthread_cond_timedwait 이 함수가 종료점이기 때문에 필요없다고 판단.
   

  주의해야될 점 : 
   1. 스레드끼리는 파일디스크립터를 공유하기 때문에 A스레드에서 파일디스크립터를 수정할 경우
   B에서도 그대로 적용되어있기 때문에
   표준 입력을 바꿔버리는 경우 표준입력을 다른 곳(ex)12) 으로 피신시켜두는것이 좋다.
   
   2  void* 형태로 데이터를 전달하고 싶으면 구조체를 활용하라!
   
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>

#define PRINT_ERR(_MSG) \
{fprintf(stderr, "%s Error\n", _MSG); exit(1);}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
typedef struct args{
    char* path;
    pthread_t td;
}Args;

void* process(void* arg);
void* close_proess (void* arg);
void clean_thread(void *arg);


int main(void)
{
    pthread_t td1, td2;
    Args args;
    int status;
    char path[] = "./hello";
    args.path = path;

    if (pthread_create(&td1, NULL, process, (void*)path) != 0)
    {
        fprintf(stderr, "pthread_open_error\n");
        exit(1);
    }
    args.td = td1;
    
    if (pthread_create(&td2, NULL, close_proess, (void*)&args) != 0)
    {
        fprintf(stderr, "pthread_open_error\n");
        exit(1);
    }


    pthread_join(td1, (void**)&status);
    pthread_join(td2, (void**)&status);

    pthread_mutex_destroy (&lock);
    pthread_cond_destroy (&cond);
}

void* process(void* arg)
{
    sleep(1);
    int fd1;
    char path[100];
    char compile[512];
    char* buf[500];
    strcpy(path, (char*)arg);
    int c;

    if( (fd1 = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
        PRINT_ERR(path);
    
/*
    pthread_cleanup_push(clean_thread, &fd1); // atexit 설정
    pthread_cleanup_pop(0);
*/
    dup2(1, 10);  // STDIN 디스크립터 10번으로 피신.
    dup2(fd1, 1); // fd를 1번으로 바꿈.
    
    // 컴파일
    sprintf(compile, "gcc -g %s.c -o %s\n", path, path);
    system(compile); //Compile 
    if (system(path) == 0)
    {
        close(fd1);
        close(1);
    }
    
    // ★ 함수가 끝났음으로 CLOSE_PROCESS에 방송호출
    pthread_cond_broadcast(&cond); 
    return NULL;
}


void* close_proess (void* arg)
{
    struct timespec timeout;
    struct timeval now;
    Args *args = (Args*)arg;
    pthread_t exec_tid = args->td;

    int err_fd;
    char path[100];
    
    strcpy(path, args->path);
    pthread_mutex_lock(&lock);
    //시간 제한
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + 6;
    timeout.tv_nsec = now.tv_usec * 1000;
    
    if (pthread_cond_timedwait(&cond, &lock, &timeout) == ETIMEDOUT)
    {
        pthread_cancel(exec_tid);
        if( (err_fd = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
                PRINT_ERR("ERROR FILE OPEN");
        dup2(2, 12); //  stderr -> 12 피신.
        dup2(err_fd, 2); //표준 에러 디스크립터 전환
        fprintf(stderr, "%s exec time out!\n",path);

        dup2(12,2); //다시 포준에러 디스크립터 전환.
        close(12);
        close(err_fd);
    }
    else
    {
        dup2(10,1);
        printf("%s Process is successful!\n", path);
        close(10);
    }
    pthread_mutex_unlock(&lock);
}

