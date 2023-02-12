#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    char line[256];
    FILE *fp;
    int uid;

    if (argc < 2)
    {
        fprintf(stderr, "usage : %s <filename> \n", argv[0]);
        exit(1);
    }

    printf("Initinally uid = %d and uid = %d\n", getuid(), geteuid());
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    { fprintf(stderr, "Fopen Error : %s \n",  argv[1]); exit(1);}
    else
    {
        printf("First Open Successful\n");
        while(fgets(line, 256, fp) != NULL)
            fputs(line, stdout);
        fclose(fp);
    }

    setuid(uid = getuid()); /* euid 를 자신의 uid로 변경해버림*/
    printf("after setuid(%d):\n uid=%d and euid=%d\n", uid,getuid(),geteuid());
    fp = fopen(argv[1], "r");

    if (fp == NULL) {fprintf(stderr, "Fopen Error 2 : %s\n", argv[1]); exit(1);}
    else
    {
        printf("Second open Successful\n");
        while(fgets(line, 256, fp) != NULL)
            fputs(line, stdout);
        fclose(fp);
    }
    exit(0);
}

/*

<처음 권한 : s설정이 되지 않아 sp2021이 파일을 읽지 못함>
junhyeong@DESKTOP-UPFPK8Q:~/go2$ ls -al setuid
-rwxrwxr-x 1 junhyeong junhyeong 19552 Feb 12 17:27 setuid

----> junhyeong
junhyeong@DESKTOP-UPFPK8Q:~/go2$ ./setuid example.txt
Initinally uid = 1000 and uid = 1000
First Open Successful
RESTRICTED AND HOPE!

after setuid(1000):
 uid=1000 and euid=1000
Second open Successful
RESTRICTED AND HOPE!
----

----> sp2021
sp2021@DESKTOP-UPFPK8Q:/home/junhyeong/go2$ ./setuid example.txt
Initinally uid = 1001 and uid = 1001
Fopen Error : example.txt
----


<사용자 euid 권한을 줌(setuid 실행파일) : u+s>
junhyeong@DESKTOP-UPFPK8Q:~/go2$ chmod u+s setuid
junhyeong@DESKTOP-UPFPK8Q:~/go2$ ls -al setuid
-rwsrwxr-x 1 junhyeong junhyeong 19552 Feb 12 17:27 setuid

<sp2021에서 실행해본 결과 : 두번째 열기에서 euid 를 자신으로 설정해버려서 오류나버림>
sp2021@DESKTOP-UPFPK8Q:/home/junhyeong/go2$ ./setuid example.txt
Initinally uid = 1001 and uid = 1000
First Open Successful
RESTRICTED AND HOPE!

after setuid(1001):
 uid=1001 and euid=1001
Fopen Error 2 : example.txt
*/
