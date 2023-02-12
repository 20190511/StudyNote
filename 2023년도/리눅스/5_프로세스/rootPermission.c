#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    int fd, state;

    state = seteuid(1000);
    if (state < 0) {fprintf(stderr, "Change EUID Error ! \n"); exit(1);}
    
    if ( (fd=open("example.txt", O_CREAT | O_RDWR, S_IRWXU)) < 0)
    {fprintf(stderr, "Open Error : example.txt\n"); exit(1);}

    close(fd);
    exit(0);
}
