#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#define MAX_NAME     MAXNAMLEN
#define ERROR_PRINT(_MSG)     \
{fprintf(stderr, "%s Error!\n",_MSG); exit(1);}

/*현재디렉토리 안의 다른폴더로 이동*/
void moving (char cur_path[], char folder_name[])
{
    printf("Current Position is %s\n", cur_path);
    DIR *dp_prev;
    struct dirent *dir;
    struct stat file_stat;
    
    dp_prev = opendir(cur_path);
    char new_path [MAX_NAME] = {"\0",};
    strcpy(new_path, cur_path);
    strcat(new_path, "/");
    strcat(new_path, folder_name);
    printf("newpath is %s \n", new_path);
    if (dp_prev == NULL)
        ERROR_PRINT("Fopen");
    
    dir = readdir(dp_prev);
    char file_name [MAX_NAME+1] = {"\0",};
    int is_existed_folder = 0; /* 폴더 명 없으면 폴더제작*/
    while (dir != NULL)
    {
        memcpy(file_name, dir->d_name, MAX_NAME);
        file_name[dir->d_reclen] = '\0';
        printf("files : %s\n", file_name);
        if (stat(file_name, &file_stat) < 0)
            ERROR_PRINT("Stat");
        if (S_ISREG(file_stat.st_mode))
            printf("[%s] type is regular File\n", file_name);
        else if (S_ISDIR(file_stat.st_mode))
        {
            printf("[%s] Type is Directory File\n", file_name);
            if (strcmp(file_name, folder_name))
                is_existed_folder = 1;
        }
        else
            printf("[%s] Type is Others!!\n", file_name);
        
        dir = readdir(dp_prev);
    }

    rewinddir(dp_prev); /*다시 처음부터 되돌림*/

    char rename_file_path [MAX_NAME] = {"\0",};
    strcat(rename_file_path, new_path);
    strcat(rename_file_path, "/");

    if (is_existed_folder)
    {
        printf("OK.. is existed! \n");
    }
    else /*dircetory 제작*/
    {
        printf("NO!\n");
        if(mkdir(new_path, S_IRUSR | S_IWUSR | S_IXUSR) < 0)
            ERROR_PRINT("Make Foler");
    }
    
    dir = readdir(dp_prev);
    char cat_new_path [MAX_NAME] = {'\0',};
    char original_path[MAX_NAME] = {'\0',};
    while (dir != NULL)
    {
        memcpy(file_name, dir->d_name, MAX_NAME);
        strcat(original_path, cur_path);
        strcat(original_path,"/");
        strcat(original_path,file_name);
        file_name[dir->d_reclen] = '\0';
        if(stat(file_name, &file_stat) < 0)
            ERROR_PRINT("moving file stat Error");
        
        if (strcmp(folder_name, file_name) && S_ISDIR(file_stat.st_mode))
            continue;
        else
        {  
            
            strcat(cat_new_path, rename_file_path);
            strcat(cat_new_path, file_name);
            if (access(cat_new_path, F_OK) != -1) /*파일 존재 확인*/
            {
                continue;
                dir = readdir(dp_prev);
                memset(cat_new_path, '\0', MAX_NAME);
                memset(original_path, '\0', MAX_NAME);
            }
            if (rename(original_path, cat_new_path) < 0)
            {
                ERROR_PRINT("Moving Error!\n");
            }
            else
                printf("Moving %s -> %s",file_name, cat_new_path);
        }
        
        dir = readdir(dp_prev);
        memset(cat_new_path, '\0', MAX_NAME);
        memset(original_path, '\0', MAX_NAME);

    }
}

int main(void){

    char dr_name[] = "newFolderess";
    int len_drname = strlen(dr_name);
    
    char cur_path [MAX_NAME];
    getcwd(cur_path, MAX_NAME);
    moving(cur_path, dr_name);
    
    

}
