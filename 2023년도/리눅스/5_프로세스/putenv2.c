#include <stdio.h>
#include <stdlib.h>

/** envpp : argv[]의 3번째 인자 타입이 ** 이므로 이 값을 그대로 가져오기 위해 ***사용. */
void ssu_printenv(char *label, char ***envpp);
void prints();
extern char** environ;

int main(int argc, char* argv[], char* envp[])
{
    /** argv 인자 복사하지않고 그대로~ 전달.*/
    ssu_printenv("Initionally", &envp);
    putenv("TZ=PST8PDT");
    ssu_printenv("After Chainging TZ", &envp);
    putenv("WARNING=Don't use envp after putenv()");
    ssu_printenv("After setting a new variable", &envp);
    printf("value of WARNING is %s\n", getenv("WARNING"));
    atexit(prints);
    printf("\n\n\n\n");
    exit(0);
}  

void ssu_printenv(char *label, char ***envpp)
{
    char **ptr;
    printf("---- %s ---\n", label);
    printf("envp is at %p and contains %p\n", envpp, *envpp);           //envpp : &envp
    printf("environ is at %p and contains %p\n", &environ, environ);
    printf("My environment variable are:");

    for (ptr = environ; *ptr != NULL ; ptr++)
    {
        printf("(%p) = %p -> %s\n",ptr,*ptr,*ptr);
    }
    printf("(%p) = %p\n",ptr,*ptr);
}

void prints()
{
    char **ptrs;
    int count = 1;
    for (ptrs=environ ; *ptrs ; ptrs++)
    {
        printf("environ[%d] address = (%p), value = %s\n",count++, *ptrs, *ptrs);
    }
}

/**
 *  :environment
 * (0x5564ad42c778) = 0x7ffc99477f6a -> HOSTTYPE=x86_64
 * (0x5564ad42c780) = 0x7ffc99477f7a -> TERM_PROGRAM=vscode
 * (0x5564ad42c788) = 0x7ffc99477f8e -> VSCODE_IPC_HOOK_CLI=/tmp/vscode-ipc-70468521-5a12-4346-8685-36d67b517338.sock
 * (0x5564ad42c790) = 0x5564ac759014 -> TZ=PST8PDT
 * (0x5564ad42c798) = 0x5564ac759038 -> WARNING=Don't use envp after putenv()
 */
