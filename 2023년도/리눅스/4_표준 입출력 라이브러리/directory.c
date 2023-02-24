/**
 * malloc size 할당 잘못해주면 이유를 알 수 없는 opendir 이런 곳에서 튕길 수 있다
 * 이유없이 튕긴다면 malloc범위를 잘 보자.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#ifdef PATH_MAX
//#define PATH_MAX        4096	/* # chars in a path name including nul */
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED    1024   

#ifndef LINE_MAX
#define LINE_MAX    2048
#endif

char *pathname;
char command [LINE_MAX], grep_cmd[LINE_MAX];

int do_grep(void)
{
    struct dirent *dirp;
    struct stat statbuf;
    char *ptr;
    DIR *dp;

    /*link 파일이면 링크된 파일이 아닌 그 자체의 buf를 가져옴*/
    if (lstat(pathname, &statbuf) < 0)
    {
        fprintf(stderr, "lstat error for %s\n", pathname);
        return 0;
    }

    /** 해당 pathname의 파일이 디렉토리가 아닌 파일이면 grep_cmd = grep -n keyword, command=pathname 이 실행되어
     *  systemp(command) 를 호출하여 터미널에 출력되게 된다.
    */
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        sprintf(command, "%s %s", grep_cmd, pathname);
        printf("%s : \n", pathname);
        system(command);
        return 0;
    }
    
    //ptr 포인터 값을 pathname 끝으로 지정함으로서 문자열포인터로서 역할을함 : 동적할당 안해줘도됨.
    ptr = pathname + strlen(pathname);
    *ptr++ = '/';
    *ptr = '\0';

    if ((dp = opendir(pathname)) == NULL)
    {
        fprintf(stderr, "Opendir Error for %s\n", pathname);
        return 0;
    }

    /** 해당 pathname 하위폴더 의 모든 요소들을 출력*/
    while ((dirp = readdir(dp)) != NULL)
    {
        // strcmp는 같으면 0을 출력한다, 다르면 1을 출력하니까 둘 다 아니면 이라는 의미
        if(strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, ".."))
        {
            strcpy(ptr, dirp->d_name);

            if(do_grep() < 0)
                break;
        }
    }

    //ptr은 현재 pathname 의 맨마지막에 위치함
    /** ptr 변천사
     *  원래 pathname = /home/junhyeong/go2
     *  *ptr++ = '/';
     *  *ptr = 0;
     *  pathname = /home/junhyeong/go2/ <-- 이렇게 되어버림
     *  여기서 ptr은 go2 다음의 / 여기로 설정되어 있음 (point1)
     *  ptr[-1] = 0; 은 go2 다음의 / 위치해 있는 ptr의 값을 '\0' 으로 만드는것이므로
     *  pathname = /home/junhyeong/go2 이 됨!
     * 
     *  ★하지만 아래 ptr[-1] 은 원본보장의 의미외에는 쓸모가 없는 라인이 되어버리는 것 같다.
     *  그러다 pathname = /home/junhyeong/go2/dir1 이라는 디렉토리가 있다면 재귀호출되어
     *  ptr이 /home/junhyeong/go2/dir1/~~~~(파일들 재귀호출하면서 바뀜) 이 모두 호출되고 while이 break되어 넘어간다면
     *  ptr[-1] 을 해주면 /home/junhyeong/go2/dir1/~~~~(마지막에 호출된 파일) ---> /home/junhyeong/go2/dir1\0~~~~~ 이 되어 
     *  삭제되고 다시 진행시키면 재귀호출이 모두 끝나서 do_grep() 위치로 되돌아 오는 순간
     *  ptr은 이전의 ptr 값인 /home/junhyeong/go2/dir1\0~~~~~ 에서 go2 다음의 / 로 건너띄어버리기(값이 바뀌기(원래대로 돌아오기)) 때문에
     *  위의 ptr[-1] 과정이 씹히게 되는 문제가 발생한다!
     * 
     *  그나마 의미 부여를 하자면 최초 호출 시 pathname이 
     *  /home/junhyeong/go2 에서 /home/junhyeong/go2/~~~~(마지막에 호출된 파일) 으로 바뀌게 되는데
     *  이때, ptr[-1]=0 을 해줌으로서 /home/junhyeong/go2\0~~~~~(마지막에 호출된 파일) 이 되어
     *  string으로서 원본이 보장될 순 있다.
     *  
     *  대안점으로는 do_grep() 내부에 static char pathname [pathmax+1] 를 해놓고
     *  strcpy를 해서 사용하면 내부적인 스택으로 원본 pathname이 붕괴되지 않도록 짤 수 있지 않을까 하는 생각이 든다.
    */
    //ptr[-1] = 0; 
    closedir(dp); //재귀호출로 인해 호출된 dp close
    return 0;
}

/**
 * grep -n keyword 경로
 * 해당 경로에 "keyword" 문자가 몇 행에 있는가 출력
*/
void make_grep (int argc, char* argv[])
{
    int i;
    strcpy(grep_cmd, " grep");
    for (i=1 ; i<argc-1 ; i++)
    {
        strcat(grep_cmd, " ");
        strcat(grep_cmd, argv[i]);
    }
}


int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <-CVchilnsvwx> <-num> <-A numm> <-B num> <-f file> \n",
        argv[0]);
        exit(1);
    }


    /** pathmax 값을 정해주는 함수 (경로의 길이)*/
    if (pathmax == 0) //pathmax 가 지정되지 않으면
    {
        /** 
         *  pathconf : 유닉스 시스템과 관련된 기본 정보와 사용자 정보 검색, 시스템의 시간을 관리하는 함수
         *  pathconf(경로, 옵션)
         *  _PC_PATH_MAX : Inquire about the value of PATH_MAX. (PATH_MAX 의 대한 정보를 알려줌)
         *  즉, 밑의 함수는 / 경로의  path_max 를 알려주는 함수다.
         *  그게 지정되어있지 않으면 MAX_PATH_GUESSED == 1024 로 대체 (위에서 정의함)한다.
         * 
         **/
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
            pathmax = MAX_PATH_GUESSED;
        else
            pathmax++;  
    }

    /** pathmax보다 1칸 많게(\0) pathname 재정의 */
    if((pathname = (char*)malloc(pathmax+1)) == NULL)
    {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    
    strcpy(pathname, argv[argc-1]);
    make_grep(argc, argv);
    do_grep();
    exit(0);

}
