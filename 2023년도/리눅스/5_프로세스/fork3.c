#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void ssu_charactertime(char *str);


int main(void)
{
    pid_t pid;

    switch (pid=fork())
    {
    case -1:
        fprintf(stderr, "Fork Error\n");
        exit(1);
        break;
    case 0:
        ssu_charactertime("Output from Child\n");
        break;
    default:
        ssu_charactertime("Output from Parent\n");
        break;
    }
    exit(0);
}


static void ssu_charactertime(char *str)
{
    char *ptr;
    int print_char;
    setbuf(stdout, NULL); //출력 버퍼 없음으로 설정

    /*print_char == 0 이되면 종료 : str이 \n 다음 값들은 0으로 세팅 */
    for (ptr=str ; print_char=*ptr ; ptr++)
    {
        putc(print_char, stdout);
        usleep(10);
    }
}
/*OOuuttppuutt  ffrroomm P aCrheinltd*/




