#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);
extern char **environ;
char glob_var[] = "HOBBY=swimming";

int main(void)
{
    int i;
    for (i=0; environ[i]!=NULL; i++)
    {
        printf("Environ[%d] : %s\n",i, environ[i]);
    }
    
    putenv(glob_var);
    /* ★★ 아직 strcpy가 일어나지 않아 당시에는, swimming 으로 되어있음.*/
    printf("environ[29] : %s\n",environ[29]);
    ssu_addone();
    printf("My hobby is %s\n", getenv("HOBBY"));
    printf("My hobby is %s\n", getenv("LOVER"));
    /* ★ glob_var는 전역변수라서 putenv를 재호출하지 않아도 환경변수값이 변경됨*/
    strcpy(glob_var+6, "fishing"); //환경변수값(전역) 변경
    
    for (i=0 ; environ[i]!=NULL ;i++)
    {
        printf("environ[%d] : %s\n",i, environ[i]);
    }

    exit(0);
}
void ssu_addone(void)
{
    /** 지역변수에서 선언된 값은 제대로 들어가지 않음.*/
    char auto_var[10];

    strcpy(auto_var, "LOVER=js");
    putenv(auto_var);
}
