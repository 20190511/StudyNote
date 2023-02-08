#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[]){
    int fd;

    if (argc != 3)
    {
        fprintf(stderr, "usage : %s <old_file> <new_file>\n", argv[0]);
        exit(1);
    }

    //1번 열어서 닫기 : 레퍼런스 카운트는 존재해서 읽고 쓸 수 있음.
    if ( (fd = open(argv[1], O_RDONLY)) < 0 )
    {
        fprintf(stderr, "open Error : %s\n", argv[1]);
        exit(1);
    }
    else
        close(fd);

    //1번 파일을 2번 파일으로 이름을 바꿈
    if (rename(argv[1], argv[2]) < 0)
    {
        fprintf(stderr, "Rename Error : %s -> %s\n", argv[1], argv[2]);
        exit(1);
    }

    if ( (fd = open(argv[2], O_RDONLY)) < 0)
    {
        fprintf(stderr, "Open Error : %s\n", argv[2]);
        exit(1);
    }

    printf("Everything is good!\n");
    return 0;
}
