#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PRINT_ERROR (_MSG) {fprint(stderr, "%s Error : linux.txt", _MSG); exit(1);)}
int main(void)
{
    char buf[64];
    char *name = "linux.txt";
    int fd;
    int lengh;

    char *msg = "";
    if (fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0600) < 0)
    {
        fprintf(stderr, "open Error \n");
        exit(1);
    }

    if (unlink(name) < 0)
    {
        fprintf(stderr, "unlink Error \n");
        exit(1);
    }

    /* unlink를 했음에도 파일에 접근 중이므로 reference count > 0 이므로 읽고 쓰기 가능*/
    if (write(fd, "How are you?", 12) != 12)
    {
        fprintf(stderr, "Write Error \n");
        exit(1);
    }

    lseek(fd, 0, SEEK_SET);

    if(lengh = read(fd, buf, sizeof(buf)) < 0)
    {
        fprintf(stderr, "Read Error \n");
        exit(1);
    }
    else{
        buf[lengh] = '\0';
    }

    printf("%s \n", buf); //출력 가능
    close(fd); /* 파일 종료 */

    /* 아래에는 fd를 닫았기 때문에 오류*/
    if ( (fd = open(name, O_RDWR)) < 0)
    {
        fprintf(stderr, "2. open Error \n");
        exit(1);
    }
    exit(0);

}
