#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage : %s <old_file> <new_file>\n",argv[0]);
        exit(1);
    }

    //argv[1] 에 대한 argv[2] 복사본 생성
    if (link(argv[1], argv[2]) < 0)
    {
        fprintf(stderr, "link Error \n");
        exit(1);
    }
    else
        printf("Step1 Successful!\n");
        
    if (remove(argv[1]) < 0)
    {  
        //오류 나면 복사본 삭제
        fprintf(stderr, "Remove Error : %s\n", argv[1]);
        remove(argv[2]); //복사본 삭제
        exit(1);
    }
    else
    {
        //원래있던 파일 지움
        remove(argv[1]);
        printf("step2 Successful!\n");
    }

    exit(0);
}
