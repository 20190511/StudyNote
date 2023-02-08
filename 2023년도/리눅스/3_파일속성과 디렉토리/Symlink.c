#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage : %s <actual_name> <symbolic_name>\n",argv[0]);
        exit(1);
    }

    if (symlink(argv[1], argv[2]) < 0)
    {
        fprintf(stderr, "SymLink error \n");
        exit(1);
    }
    else
    {
        printf("Symlink : %s -> %s\n", argv[2], argv[1]);
    }


    return 0;
}
