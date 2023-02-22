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

/** argv[1]
 *
 *  실험결과 : 디렉토리가 비어있어도 이동가능 (이름 잘못정함 ㅋㅋ)
 * ./moving                         -> temp name 폴더를 생성해서 모든 파일 이동 (.vscode, moving 제외)
 * ./moving <folder_name>           -> 현재 폴더에 folder_name 생성하고 모든 파일 이동 (.vscode, moving 제외)
 * ./moving <path> <folder_name>    -> 해당 path에 folder_name 생성하고 모든 파일 이동 (.vscode, moving 제외)
 * 환경변수화 + CTRL+C 눌렀을 때 
*/
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

    char original_path[MAX_NAME*2] = {'\0',};
    while (dir != NULL)
    {
        strcpy(file_name, dir->d_name);
        file_name[dir->d_reclen] = '\0';
        strcat(original_path, cur_path);
        strcat(original_path,"/");
        strcat(original_path,file_name);
        printf("files : %s\n", original_path);

        if (stat(original_path, &file_stat) < 0)
            ERROR_PRINT("Stat");
        if (S_ISREG(file_stat.st_mode))
            printf("[%s] type is regular File\n", file_name);
        else if (S_ISDIR(file_stat.st_mode))
        {
            printf("[%s] Type is Directory File\n", file_name);
            if (strcmp(file_name, folder_name) == 0)
                is_existed_folder = 1;
        }
        else
            printf("[%s] Type is Others!!\n", file_name);
        
        dir = readdir(dp_prev);
        memset(original_path, '\0', MAX_NAME);
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
    
    /* 파일 이동*/
    dir = readdir(dp_prev);
    char cat_new_path [MAX_NAME] = {'\0',};

    while (dir != NULL)
    {
        strcpy(file_name, dir->d_name);
        file_name[dir->d_reclen] ='\0';
        printf("%s\n",file_name);
        strcat(original_path, cur_path);
        strcat(original_path,"/");
        strcat(original_path,file_name);
        file_name[dir->d_reclen] = '\0';
        if(stat(original_path, &file_stat) < 0)
            ERROR_PRINT("moving file stat Error");
        
        //★ 주의 strcmp는 같으면 0이다 ! 조심해라
        if (((strcmp(folder_name, file_name) == 0) ||
            (strcmp(".vscode", file_name) == 0 )) && S_ISDIR(file_stat.st_mode))
        {
            dir = readdir(dp_prev);
            memset(cat_new_path, '\0', MAX_NAME);
            memset(original_path, '\0', MAX_NAME);
            continue;
        }
        else if (((strcmp("moving.c", file_name) == 0) ||
                    (strcmp("moving", file_name) == 0)) && S_ISREG(file_stat.st_mode))
        {
            dir = readdir(dp_prev);
            memset(cat_new_path, '\0', MAX_NAME);
            memset(original_path, '\0', MAX_NAME);
            continue;
        }
        else
        {  
            
            strcat(cat_new_path, rename_file_path);
            strcat(cat_new_path, file_name);
            if (access(cat_new_path, F_OK) != -1) //파일 존재 확인
            {
                dir = readdir(dp_prev);
                memset(cat_new_path, '\0', MAX_NAME);
                memset(original_path, '\0', MAX_NAME);
                continue;
            }
            if (rename(original_path, cat_new_path) < 0)
            {
                printf("%s -> %s",original_path, cat_new_path);
                ERROR_PRINT("Moving Error!\n");
            }
            else
                printf("Moving %s -> %s\n",file_name, cat_new_path);
        }
        
        dir = readdir(dp_prev);
        memset(cat_new_path, '\0', MAX_NAME);
        memset(original_path, '\0', MAX_NAME);

    }
}

int main(int argc, char* argv[]){

    if (argc > 3)
    {
        fprintf(stderr, "usage : %s \n or %s <moving Folder name> or \n %s <moving path> <moving Folder name>\n",  argv[0], argv[0], argv[0]);
        exit(1);
    }

    if (argc == 2)
    {
        char cur_path [MAX_NAME];
        getcwd(cur_path, MAX_NAME);
        moving(cur_path, argv[1]);
    }
    if (argc == 3)
    {
        moving(argv[1], argv[2]);
    }
    if (argc == 1)
    {
        /*tmpnam 으로 생성*/
        char tmp_val [L_tmpnam];
        char* tmp_check = tmpnam(tmp_val);
        if (tmp_check == NULL)
            ERROR_PRINT("temp file making ");
        //strtok 는 token을 /0 으로 만들고 다음 값으로 가리킴
        char *tmp_name = strtok(tmp_check, "/");
        tmp_name = strtok(NULL, "/");

        char cur_path [MAX_NAME];
        getcwd(cur_path, MAX_NAME);
        moving(cur_path, tmp_name);
    }
    
    
    exit(0);
}
