#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void)
{
    pid_t pid;

    if((pid = fork()) < 0) {fprintf(stderr, "Fork Error\n"); exit(1);}
    else if (pid != 0)
    {
        exit(1); //부모 프로세스는 바로 종료
    }

    /* getsid(0) 과 getsid(getpid()) 는 동일*/    
    printf("Before pid = %d, session id(SID) = %d\n",getpid(), getsid(0));
    printf("Process Group Leader = %d's = %d\n",getpid(), getpgid(0));
    setsid(); /*자식 프로세스 pid가 session 리더 + 프로세스 그룹리더가됨.*/
    printf("After pid = %d, session id(SID) = %d\n",getpid(), getsid(0));
    printf("Process Group Leader = %d's = %d\n",getpid(), getpgid(0));
    exit(0);


}
/*
Before pid = 2813, session id(SID) = 1870
Process Group Leader = 2813's = 2812
After pid = 2813, session id(SID) = 2813
Process Group Leader = 2813's = 2813
*/
