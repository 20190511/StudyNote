#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    struct stat buf;
    char *str;
    int i;

    for (i = 1 ; i < argc ; i++)
    {
        printf("name=%s, types=",argv[i]);

        // 링크파일도 파일 자체에 접근 
        if (lstat(argv[i], &buf) < 0)
        {
            fprintf(stderr, "File Error : %s", argv[i]);
            continue;
        }
        printf("%x : ",buf.st_mode);
        //st_mode 를 매크로로 돌려서 타입 알아냄 : #define	S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)
        if (S_ISREG(buf.st_mode))
            str = "regular";
        else if (S_ISDIR(buf.st_mode))
            str = "Dictionary";
        else if (S_ISCHR(buf.st_mode))
            str = "Charter Special";
        else if (S_ISBLK(buf.st_mode))
            str = "Block Special";
        else if (S_ISFIFO(buf.st_mode))
            str = "FIFO";
        else if (S_ISLNK(buf.st_mode))
            str = "Symbolic Link";
        else if (S_ISSOCK(buf.st_mode))
            str = "Socket";
        else
            str = "Unknown FIle";

        printf("%s\n", str);
    }
    return 0;
}
