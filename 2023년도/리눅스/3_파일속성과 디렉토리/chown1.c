#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    struct stat buf;
    char *name = "/home/junhyeong/ssu_myfile";
    int fd;

    if(fd = open(name, O_RDWR | O_CREAT, 0600) < 0)
    {
        fprintf(stderr, "open error %s", name);
        exit(1);
    }

    close(fd);
    stat(name, &buf);
    printf("# 1st stat call # UID:%d, GID:%d\n", buf.st_uid, buf.st_gid);

    //해당 파일 gid, uid 변경.
    if (chown(name, 501, 300) < 0)
    {
        fprintf(stderr, "Chown error for %s\n", name);
        exit(1);
    }    
    stat(name, &buf);
    printf("# 2nd stat call # UID:%d, GID:%d\n", buf.st_uid, buf.st_gid);

    if (unlink(name) < 0)
    {
        fprintf(stderr, "Unlink Error for %s\n", name);
        exit(1);
    }
    exit(0);
}
