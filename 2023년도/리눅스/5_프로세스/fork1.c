#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char glob_str[] = "write to standard Output\n";
int glob_val = 10;

int main(void)
{
    pid_t pid;
    int loc_val;
    loc_val = 100;

    if (write(1, glob_str, sizeof(glob_str)-1) != sizeof(glob_str)-1)
    {
        fprintf(stderr, "STDOUT ERROR \n");
        exit(1);
    }

    printf("before Fork!\n");

    switch(pid=fork())
    {
        case -1:
            fprintf(stderr,"Fork Error");
            exit(1);
            break;
        case 0:
            /*자식 프로세스 지역,전역 변수 수정 <- 자식프로세스만 영향미침*/
            glob_val++;
            loc_val++;
            break;
        default:
            sleep(3);
            break;
    }

    printf("Pid=%d, GPID=%d, glob_val=%d, loc_val=%d\n", getpid(), getpgid(getpid()),glob_val, loc_val);
    exit(0);

}
/*
write to standard Output
before Fork!
Pid=2607, PGID=2605, glob_val=11, loc_val=101
Pid=2605, PGID=2605, glob_val=10, loc_val=100
*/
