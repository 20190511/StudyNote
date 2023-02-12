#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>

void ssu_do_cmd(char *cmd);
void ssu_print_times(clock_t real, struct tms *tms_start, struct tms *tms_end);
void ssu_echo_exit(int status);

int main (int argc, char *argv[])
{
    int i;
    setbuf(stdout, NULL); /*Buffer 없애버림*/
    
    for (i=1 ; i<argc; i++)
        ssu_do_cmd(argv[i]);
    exit(0);
}

void ssu_do_cmd(char *cmd)
{
    struct tms tms_start, tms_end;
    clock_t start, end; //return 받을 값
    int status;

    // 시작 시점 클락 틱 수 측정
    if ((start=times(&tms_start)) == -1)
    {fprintf(stderr, "Start times Error\n"); exit(1);}

    /* cmd 수행 -> status 반환*/
    if (status = system(cmd) < 0)
    {fprintf(stderr, "System Error : %s\n", cmd); exit(1);}

    // 종료 시점 클락 틱 수 측정
    if ((end=times(&tms_end)) == -1)
    {fprintf(stderr, "End times Error\n"); exit(1);}

    /* real time 은 end-start tick수로 구함.*/
    ssu_print_times(end-start, &tms_start, &tms_end);
    ssu_echo_exit(status);
}
void ssu_print_times(clock_t real, struct tms *tms_start, struct tms *tms_end)
{
    static long clocktick = 0; //초 당 클락 틱 수

    if (clocktick == 0)
    {   /* 초당 클락 틱 수 = sysconf(_SC_CLK_TCK) // sysconf() : 시스템 자원 정보 검색 함수.*/
        if ((clocktick = sysconf(_SC_CLK_TCK)) < 0) {fprintf(stderr, "Sysconf Error\n"); exit(1);}
    }
    printf(" real : %7.2f\n", real/(double)clocktick);
    printf(" user : %7.2f\n", (tms_end->tms_utime - tms_start->tms_utime)/(double)clocktick);
    printf(" system : %7.2f\n", (tms_end->tms_stime - tms_start->tms_stime)/(double)clocktick);
    printf(" Child user : %7.2f\n", (tms_end->tms_cutime - tms_start->tms_cutime)/(double)clocktick);
    printf(" Child system : %7.2f\n", (tms_end->tms_cstime - tms_start->tms_cstime)/(double)clocktick);
}
void ssu_echo_exit(int status)
{
    if(WIFEXITED(status))
    {
        printf("Normal Terminate = %d \n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("abnormal Terminate : Signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? "CoredDumped!" : "Not Cored Dump!");
#else
        "");
#endif
    }
    else if (WIFSTOPPED(status))
    {
        printf("Child Stopped : Signal number = %d\n", WSTOPSIG(status));
    }
}
/**
 * junhyeong@DESKTOP-UPFPK8Q:~/go2$ ./times "sleep 5" date
 real :    5.00
 user :    0.00
 system :    0.00
 Child user :    0.00
 Child system :    0.00
Normal Terminate = 0
Sun Feb 12 19:48:07 KST 2023
 real :    0.00
 user :    0.00
 system :    0.00
 Child user :    0.00
 Child system :    0.00
Normal Terminate = 0
*/
