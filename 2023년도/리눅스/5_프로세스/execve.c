#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char *argv[]={
        "execl", "ssu_test1", "ssu_test2", "ssu_test3", (char*)NULL
    };

    char *env[] = {
        "NAME=value",
        "NEXTNAME=nextvalue",
        "HOME=/HOME/go2",
        (char*)0
    };

    printf("This is the original Program\n");
    execve("./execl", argv, env);
    /*아래부터는 자식프로세스가 부모프로세스를 덮어써버려서 호출불가.*/
    printf("This line is never called\n");
    exit(0);
}
/*
This is the original Program
argv[0]=execl
argv[1]=ssu_test1
argv[2]=ssu_test2
argv[3]=ssu_test3
NAME=value
NEXTNAME=nextvalue
HOME=/HOME/go2
*/
