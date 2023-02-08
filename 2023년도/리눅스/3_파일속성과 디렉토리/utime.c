/* 파일을 수정하더라도 수정시간을 변경시키지 않는 함수.*/
#include <sys/types.h>
#include <utime.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    struct utimbuf utim_buf;
    struct stat stat_buf;
    int fd,i;

    for (i = 1 ; i < argc ; i++)
    {
        /* stat() 으로 미리 m,atime 을 받아옴.*/
        if (stat(argv[i], &stat_buf) < 0)
        {
            fprintf(stderr, "Stat Error : %s\n", argv[i]);
            continue;
        }

        if ( (fd = open(argv[i], O_RDWR | O_TRUNC)) < 0)
        {
            fprintf(stderr, "Open Error : %s\n", argv[i]);
            continue;
        }

        close(fd);
        /* open 하기 전 stat_buf 를 통해 받아놓은 시간*/
        utim_buf.actime = stat_buf.st_atime;
        utim_buf.modtime = stat_buf.st_mtime;

        if (utime(argv[i], &utim_buf) < 0)
        {
            fprintf(stderr, "utime error for %s\n", argv[i]);
            continue;
        }
    }
    exit(0);
}
