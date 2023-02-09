#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(void)
{
    char *name = "ssu_test.txt";
    char *mode = "r";  //읽기 모드

    int fd;
    if (fd = open(name, O_RDONLY) < 0)
    {
        fprintf(stderr,"error\n");
    }
    else
    {
        printf("Good\n");
    }


    if (fopen(name, mode) == NULL)
    {
        fprintf(stderr, "fopen Error : %s\n", name);
        exit(1);
    }
    else
        printf("Success\n Filename:<%s>, mode:<%s>\n", name, mode);
        exit(0);
}
