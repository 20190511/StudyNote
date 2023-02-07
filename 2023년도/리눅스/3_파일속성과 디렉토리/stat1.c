#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int main (int argc, char*argv[])
{
    struct stat statbuf;
    if (argc != 2)
    {
        fprintf(stderr, "usage : %s <file>\n", argv[0]);
        exit(1);
    }

    if ( (stat(argv[1], &statbuf)) < 0)
    {
        fprintf(stderr, "stat error \n"); // 46Bytes
        exit(1);
    }

    printf("%s is %ld bytes\n", argv[1], statbuf.st_size);
    exit(0);
}
