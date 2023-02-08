/* VSCode 에서는 한글 디렉토리 생성 시 출력이되지않는 오류가 있음 -> 영어로 디렉토리 만들 것.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX    1024

int main (void)
{
    char  *name;
    name = malloc(PATH_MAX);    //동적할당
    getcwd(name,PATH_MAX);
    printf("Current Directory : %s\n", name);
    if (chdir("/home/junhyeong/") == -1)
    {
        fprintf(stderr, "Change Directory Error\n");
        exit(1);
    }
    /* 현재 디렉토리 경로를 받아옴.*/
    char  *name2;
    name2 = malloc(PATH_MAX);    //동적할당
    if (getcwd(name2, PATH_MAX) == NULL)
    {
        fprintf(stderr, "Name Error\n");
        exit(1);
    }

    printf("Changed + Current Directory : %s\n",name2);
    exit(0);
}
