#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#define  MAX_NAME    MAXNAMLEN

struct dirent *dir_struct;
int main(void)
{

    char cur_name[MAX_NAME];
    getcwd(cur_name, MAX_NAME);
    char new_name[] = "/newFoldersss"; /*고정문자열인 경우 되도록이면 동적배열 사용 금할것*/
    strcat(cur_name,new_name); /*cur_name에 cur_name+new_name이 자동으로 됨*/


    if (mkdir(cur_name, S_IRUSR | S_IWUSR | S_IXUSR)<0)
    {
        fprintf(stderr, "make Folder Error\n");
        exit(1);
    }

    struct dirent *dir;
    char cur_folder[MAX_NAME];
    getcwd(cur_folder, MAX_NAME);  /*현재 주소값 받아오기 unistd.h*/
    DIR *dp = opendir(cur_folder);
    struct stat *stat_s;
    if (dp == NULL)
    {
        fprintf(stderr, "Fopen Error\n");
        exit(1);
    }
    dir = readdir(dp);
    char filename[MAX_NAME+1];
    while (dir != NULL)
    {
        memcpy(filename, dir->d_name, dir->d_reclen); /*dir->d_reclen 보다 파일값이 작을 수 있으므로 MAX_NAME 을 넣는 것이 바람직함*/
        filename[dir->d_reclen] = '\0'; // d_reclen : 파일이름 크기
        dir = readdir(dp);

        if(stat(filename, stat_s) < 0)
        {
            fprintf(stderr, "fstatError : %s\n", filename);
            exit(1);
        }
        else
        {
            if (S_ISREG(stat_s->st_mode))
                printf("%s is regualr file\n", filename);
            else if (S_ISDIR(stat_s->st_mode))
                printf("%s is directory file\n", filename);
        }
    }
}
/*

newFolders is directory file
newFolder is directory file
rading.c is regualr file
helloWorld is regualr file
directory.c is regualr file
test.c is regualr file
rading is regualr file
newFoldersss is directory file
test.txt is regualr file
newFolderss is directory file
test is regualr file
directory is regualr file
.vscode is directory file
.. is directory file
helloWorld.c is regualr file
. is directory file
*/
