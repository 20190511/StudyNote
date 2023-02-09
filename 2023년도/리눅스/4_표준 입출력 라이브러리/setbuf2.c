#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE     1024

int main (void)
{
    char buf [BUFFER_SIZE];
    memset (buf, '\n', BUFFER_SIZE);
    int a,b;
    int i;

    setbuf(stdin, buf);
    scanf("%d %d",&a, &b);

    for (i = 0 ; buf[i] != '\n' ;i++)
    {
        //화면에 buf 출력
        putchar(buf[i]);
    }
    
    putchar('\n');
    exit(0);
}
