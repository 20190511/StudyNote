/* freopen(이동할 fp, 모드, 이동시킬 fp ex)stdin,stdout )*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    char *name = "ssu_test.txt";
    int fd;
    FILE *fp;

    printf("First Printf : Hello\n");

    if ((fd=open(name, O_RDONLY)) < 0)
    {
        fprintf(stderr, "Open Error : %s\n", name);
        exit(0);
    }

    // freopen 함수를 이용하여 stdout(모니터출력) 을 파일로 바꿔버림.
    if ((fp = freopen(name, "w", stdout)) != NULL)
    {
        printf("Second printf : Hello!\n");
    }
    else
    {
        fprintf(stderr, "ERROR!\n");
    }
    exit(0);
}
/*
First Printf : Hello

Second printf는 
*/
