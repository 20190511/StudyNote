#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

/*timeval :초 /마이크로초 구조체 */
double ssu_maketime (struct timeval *time);
void term_stat(int stat);
void ssu_print_child_info (int stat, struct rusage *rusage);

int main (void)
{
    struct rusage rusage;
    pid_t pid;
    int status;

    if((pid=fork()) == 0)
    {  
        /*자식 프로세스에서 find 실행*/
        char *args[] = {"find", "/", "-maxdepth", "4", "-name", "stdio.h", (char*)NULL};

        if (execv("/usr/bin/find", args) < 0)
        { fprintf(stderr, "excution Error\n"); exit(1);}
    }


    /* 자식 프로세스에 대한 resource 정보를 얻어옴 : Options : 0(기본값)*/
    if (wait3(&status, 0, &rusage) == pid)
    {
        ssu_print_child_info(status, &rusage);
    }
    else
    {
        fprintf(stderr, "wait3 Error\n");
        exit(1);
    }

    exit(0);
}
double ssu_maketime (struct timeval *time)
{
    /*초.마이크로초 설정.*/
    return ((double)time->tv_sec + (double)time->tv_usec/1000000.0);
}

/** 자식 프로세스가 종료된 후 종료 status 분석
 * 정상종료 -> WIFEXITED
 * 시그널발생 -> WIFSIGNALED
 * 시그널중단 -> WIFSTOPPED
*/
void term_stat(int status)
{
    if(WIFEXITED(status))
    {
        printf("Normally Terminated Exit status = %d\n", WEXITSTATUS(status));
    }
    else if  ((WIFSIGNALED(status)))
    {
        printf("Abnormal termination by signal %d. %s\n", WTERMSIG(status),
#ifdef WCOREDUMP
    WCOREDUMP(status) ? "CORE DUMPED" : "NO DUMPED");
#else
    "");
#endif
    }
    else if (WIFSTOPPED(status))
    {
        printf("Stopped by signal%d\n", WSTOPSIG(status));
    }
}

/** 자식 프로세스 (사용자/시스템) CPU Time을 초.마이크로초로 알려주는함수.
 *  해당 rusage를 그대로 넘겨줌 -> rusage
 *  ssu_maketime()역시 해당 포인터를 그대로 넘겨줘야하므로 &를 붙여야한다.
 * 
 */
void ssu_print_child_info (int stat, struct rusage *rusage)
{
    printf("Terminated info follows\n");
    term_stat(stat);
    printf("user CPU Time : %.2f(sec)\n", ssu_maketime(&rusage->ru_utime));
    printf("System CPU Time : %.2f(sec)\n", ssu_maketime(&rusage->ru_stime));
}

/*
find: ‘/home/sp2021’: Permission denied
find: ‘/etc/polkit-1/localauthority’: Permission denied
find: ‘/etc/ssl/private’: Permission denied
find: ‘/root’: Permission denied
find: ‘/mnt/d/System Volume Information’: Permission denied
find: ‘/mnt/c/Recovery’: Permission denied
find: ‘/mnt/c/System Volume Information’: Permission denied
/usr/include/stdio.h
find: ‘/proc/tty/driver’: Permission denied
find: ‘/proc/1/fd’: Permission denied
find: ‘/proc/1/map_files’: Permission denied
find: ‘/proc/1/fdinfo’: Permission denied
find: ‘/proc/1/ns’: Permission denied
find: ‘/proc/8/fd’: Permission denied
find: ‘/proc/8/map_files’: Permission denied
find: ‘/proc/8/fdinfo’: Permission denied
find: ‘/proc/8/ns’: Permission denied
find: ‘/proc/9/fd’: Permission denied
find: ‘/proc/9/map_files’: Permission denied
find: ‘/proc/9/fdinfo’: Permission denied
find: ‘/proc/9/ns’: Permission denied
find: ‘/proc/142/fd’: Permission denied
find: ‘/proc/142/map_files’: Permission denied
find: ‘/proc/142/fdinfo’: Permission denied
find: ‘/proc/142/ns’: Permission denied
find: ‘/proc/143/fd’: Permission denied
find: ‘/proc/143/map_files’: Permission denied
find: ‘/proc/143/fdinfo’: Permission denied
find: ‘/proc/143/ns’: Permission denied
find: ‘/proc/420/fd’: Permission denied
find: ‘/proc/420/map_files’: Permission denied
find: ‘/proc/420/fdinfo’: Permission denied
find: ‘/proc/420/ns’: Permission denied
find: ‘/proc/421/fd’: Permission denied
find: ‘/proc/421/map_files’: Permission denied
find: ‘/proc/421/fdinfo’: Permission denied
find: ‘/proc/421/ns’: Permission denied
find: ‘/proc/429/fd’: Permission denied
find: ‘/proc/429/map_files’: Permission denied
find: ‘/proc/429/fdinfo’: Permission denied
find: ‘/proc/429/ns’: Permission denied
find: ‘/proc/430/fd’: Permission denied
find: ‘/proc/430/map_files’: Permission denied
find: ‘/proc/430/fdinfo’: Permission denied
find: ‘/proc/430/ns’: Permission denied
find: ‘/var/spool/rsyslog’: Permission denied
find: ‘/var/lib/polkit-1’: Permission denied
find: ‘/var/lib/private’: Permission denied
find: ‘/var/cache/ldconfig’: Permission denied
find: ‘/var/cache/private’: Permission denied
find: ‘/var/log/private’: Permission denied
find: ‘/lost+found’: Permission denied
Terminated info follows
Normally Terminated Exit status = 1
user CPU Time : 0.03(sec)
System CPU Time : 0.06(sec)
*/
