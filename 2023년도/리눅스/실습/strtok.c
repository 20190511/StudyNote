#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char path[] = "as/home/junhyeong/go2";        // 첫 strtok 때 as 가 들어감
    char path2[] = "/home/junhyeong/go2";         // 첫 strtok 때 home 이 들어감
    //아래처럼 돌리면 안돌아감. <-- strtok에 할당되어진 문자열을 넣어야지 썡 문자열을 박아서 포인터로 가리키는 형태로 적으면 안됨.
    //char *path = "as/home/junhyeong/go2";
    char *idx[100];
    int idx_cnt = 0;
    
    char *token = strtok(path,"/");
    while (token != NULL)
    {
        idx[idx_cnt++] = token;
        token = strtok(NULL, "/");
    }
    
    for (int i = 0 ; i < idx_cnt ; i++)
        printf("%s\n", idx[i]);
        
    return 0;
}
