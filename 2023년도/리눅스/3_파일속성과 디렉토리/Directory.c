/* opendir() -> DIR *dp -> readdir() -> dirent -> stat 세부정보 출력 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE  MAXNAMLEN //255

int main (int argc, char* argv[])
{
    char file_name[DIRECTORY_SIZE+1];
    struct dirent *dentry;
    struct stat statbuf;
    DIR *dp;

    if (argc < 2)
    {
        fprintf(stderr, "usage : %s <directory>\n", argv[0]);
        exit(1);
    }

    if ((dp = opendir(argv[1]))==NULL || chdir(argv[1]) == -1)
    {
        fprintf(stderr,"Opendir and setting Error : %s\n", argv[1]);
        exit(1);
    }

    //dp로부터 해당 디렉토리 안의 파일을 하나씩 받아옴
    while ( (dentry = readdir(dp)))
    {
        if (dentry->d_ino == 0)
            continue;

        //해당 파일의 이름을 저장
        memcpy(file_name, dentry->d_name, DIRECTORY_SIZE);
        //파일의 세부적인 정보를 받아옴.
        if (stat(file_name, &statbuf) < 0)
        {
            fprintf(stderr,"Stat Error : %s\n", file_name);
            continue;
        }

        if( S_ISREG(statbuf.st_mode))
        {
            printf("%-14s, size=%ld\n",file_name, statbuf.st_size);
        }
        else
        {
            printf("%-14s\n",file_name);
        }
    }

    exit(0);
}
/*
<output>
junhyeong@DESKTOP-UPFPK8Q:~/go/Books/파일 속성과 디렉토리$ ./Directory /home/junhyeong/
go/
.
..
.vscode
a.out         , size=16216
blank.c       , size=47469
Books
DataStructure
Hello world.txt, size=0
link1.txt     , size=17
LinuxPractice
Practice      , size=0
Practice.c    , size=569
Practice.exe  , size=132385
Project
ssu_myfile    , size=0
ssu_test.txt  , size=59
ubuntu-22.10-desktop-amd64.iso, size=4071903232
*/
