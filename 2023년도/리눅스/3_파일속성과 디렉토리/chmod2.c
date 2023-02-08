#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define MODE_EXEC (S_IXUSR | S_IXGRP | S_IXOTH)

int main (int argc, char* argv[])
{
    struct stat buf;
    int i;

    if (argc < 2 )
    {
        fprintf(stderr, "usage : %s <file> <file> ...", argv[0]);
        exit(1);
    }

    for (i=1 ; i<argc ; i++)
    {
        if (stat(argv[i], &buf) < 0)
        {
            fprintf(stderr, "%s stat err\n", argv[i]);
            continue;
        }

        buf.st_mode |= MODE_EXEC;
        buf.st_mode ^= (S_IXGRP | S_IXOTH);
        if (chmod(argv[i], buf.st_mode) < 0)
        {
            fprintf(stderr, "%s : chmod Error\n", argv[i]);
        }
        else
            printf("%s : file permission was changed\n", argv[i]);
    }
    exit(0);
}
