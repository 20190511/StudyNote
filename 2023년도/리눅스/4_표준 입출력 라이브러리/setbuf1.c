/* */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);

int main (void)
{
    char buf[BUFFER_SIZE];
    char *name = "/dev/pts/6"; //Linux 터미널을 fp로 받아옴.
    FILE *fp;

    if ( (fp = fopen(name, "w")) == NULL)
    {
        fprintf(stderr, "Fopen Error : %s\n", name);
        exit(1);
    }

    /*쉘에 한번에 출력됨*/
    ssu_setbuf(fp, buf);
    fprintf(fp, "Hello, ");
    sleep(1);
    fprintf(fp, "UNIX ");
    sleep(1);
    fprintf(fp, "\n");
    sleep(1);

    /*쉘에 하나씩 출력됨*/
    ssu_setbuf(fp,NULL);
    fprintf(fp, "How, ");
    sleep(1);
    fprintf(fp, "Are, ");
    sleep(1);
    fprintf(fp, "You!, ");
    sleep(1);
    fprintf(fp, "\n");
    sleep(1);
    exit(0);
}

void ssu_setbuf(FILE *fp, char *buf)
{
    size_t size;
    int fd;
    int mode;
    fd = fileno(fp); //Return the system file descriptor for STREAM.
    
    if (isatty(fd)) //fd가 terminal이면 1(true), 아니면 0(false)
        mode = _IOLBF;
    else
        mode = _IOFBF;
    
    if (buf == NULL)
    {
        mode = _IONBF;
        size = 0;
    }
    else
        size = BUFFER_SIZE;
    
    setvbuf(fp, buf, mode, size);
}
