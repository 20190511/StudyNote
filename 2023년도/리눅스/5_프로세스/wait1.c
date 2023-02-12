#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
void ssu_echo_exit (int status);

int main(void)
{
    pid_t pid;
    int status;

    /* 1번째 자식 프로세스 : 종료후 satatus 7 전달 .*/
    if ((pid = fork()) < 0)
    { fprintf(stderr, "Fork Error\n") ; exit(1);}
    else if (pid == 0) //자식 프로세스 종료
    { exit(7);}
    
    if (wait(&status) != pid)
    { fprintf(stderr, "wait Error\n"); exit(1);}

    ssu_echo_exit(status);

    /* 2번째 자식 프로세스 : abort() 호출 .*/
    if ((pid = fork()) < 0)
    { fprintf(stderr, "Fork Error\n") ; exit(1);}
    else if (pid == 0)
    { abort();}
    if (wait(&status) != pid)
    { fprintf(stderr, "wait Error\n"); exit(1);}

    ssu_echo_exit(status);

    /* 3번쨰 자식 프로세스 : status /= 0 (status 0로 나눔 : DIVISION 오류 초래)*/
    if ((pid = fork()) < 0)
    { fprintf(stderr, "Fork Error\n") ; exit(1);}
    else if (pid == 0) //자식 프로세스 종료
    { status /= 0;}
    
    if (wait(&status) != pid)
    { fprintf(stderr, "wait Error\n"); exit(1);}
    ssu_echo_exit(status);

    exit(0);

}



void ssu_echo_exit (int status)
{
    /*정상 종료시 참*/
    if (WIFEXITED(status))
    {
        printf("Normal Termination, exit status = %d\n", WEXITSTATUS(status));
    }
    /** WCROEDUMP 상수가 정의되어있으면
     * 해당 상수가 참이면 -> "Core file generated", 거짓이면 ""
     *  WCOREDUMP 상수가 정의x면
     * ""
     */
    else if (WIFSIGNALED(status))
    {
        printf("abnormal terminate, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP 
    WCOREDUMP(status) ? "(Core file generated)" : "");
#else
    "");
#endif
    }
    /* 자식프로세스가 멈췄으면 해당 시그널 출력*/
    else if (WIFSTOPPED(status))
    {
        printf("Child Stopped, signal Number = %d",WSTOPSIG(status));
    }
}

/*
ormal Termination, exit status = 7
abnormal terminate, signal number = 6
abnormal terminate, signal number = 8
*/
