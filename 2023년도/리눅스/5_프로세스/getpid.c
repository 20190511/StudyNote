#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    pid_t pgid;
    pid_t pid;

    if (argc < 2) {fprintf(stderr, "usage : %s <pid>\n",argv[0]); exit(1);}
    pid = getpid();
    pgid = getpgid(atoi(argv[1])); /*해당 인자 pid의 그룹pid 리턴*/

    printf("pid = %d, pgid = %d\n", pid,pgid);
    exit(0);
}


/**
 *

        │                                   ├─{node}(435)
        │                                   ├─{node}(436)
        │                                   └─{node}(437)
        └─{init}(6)
junhyeong@DESKTOP-UPFPK8Q:~/go2$ ./getpgid 6
pid = 2355, pgid = 0
*/
