/* 실헹 상태에서 다른 터미널 켜서 파일 수정해야함..*/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
    
    
struct stat buf;
void ssu_checkfile(char *fname, time_t *time);

int main (int argc, char* argv[])
{

    time_t intertime;
    if (argc != 2 )
    {
        fprintf(stderr, "usage %s <file>\n", argv[0]);
        exit(1);
    }

    if (stat(argv[1], &buf) < 0)
    {
        fprintf(stderr, "stat error !\n");
        exit(1);
    }
    
    intertime = buf.st_mtime; //최종 수정 시간.
    int cnt = 0;
    while (1)
    {
        ssu_checkfile(argv[1], &intertime);
        sleep(10);
    }
}

void ssu_checkfile(char *fname, time_t *time)
{
    if (stat(fname, &buf) < 0)
    {
        fprintf(stderr, "statError \n");
        exit(1);
    }
    else{
        if (buf.st_mtime != *time) //최종 수정 시간과 수정시간들이 같지 않으면.
        {
            printf("Warning : %s was modified. \n", fname);
            *time = buf.st_mtime;
        }
    }
}
