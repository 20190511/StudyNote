#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define RW_MODE (S_IRUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH)
#define PRINT_ERROR(_FILE) {fprintf(stderr, "creat error : %s", _FILE); exit(1);}
int main (void)
{
    char *name1 = "/home/junhyeong/ssu_test1.txt";
    char *name2 = "/home/junhyeong/ssu_test2.txt";

    umask(0);

    if ( creat(name1, RW_MODE) < 0)
        PRINT_ERROR(name1);
    umask( S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if ( creat(name2, RW_MODE) <0)
        PRINT_ERROR(name2);
    
    exit(0);
}
