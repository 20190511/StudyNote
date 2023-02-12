#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{
    if (fork() == 0)
        execl("/bin/echo", "echo", "this is", "message one", (char*)NULL);
    
    if (fork() == 0)
        execl("/bin/echo", "echo", "this is", "message two", (char*)0);
        
    printf("parent : wait Child Process\n");

    /*자식 프로세스 status 안받음 NULL 넣기 (int*)NULL == (int*)0*/
    while (wait((int*)0) != -1);

    printf("parent : all children terminated\n");
    exit(0);
}
/*
parent : wait Child Process
this is message two
this is message one
parent : all children terminated
*/

