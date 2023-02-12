#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

/** 문자열화 연산자 : https://learn.microsoft.com/ko-kr/cpp/preprocessor/stringizing-operator-hash?view=msvc-170
 *  ssu_show_limit(limit,#limit) <- #limit 는 limit 변수를 문자열화 해서 넣겠다는 의미
 */
#define ssu_show(limit)     ssu_show_limit(limit, #limit) 
void ssu_show_limit(int rlim_type, char *rlim_name);

int main (void)
{
    printf("    resource soft limit hard limit      \n\n");
    ssu_show(RLIMIT_CPU);       /*프로세스가 소비할 수 있는 최대 cpu 시간 (초)*/
    ssu_show(RLIMIT_FSIZE);     /*파일 생성 시 파일 최대 크기 (바이트)*/
    ssu_show(RLIMIT_DATA);      /*자료 구역 전체의 최대 크기 (바이트)*/
    ssu_show(RLIMIT_STACK);     /* 스택 최대 크기 (바이트)*/
    ssu_show(RLIMIT_CORE);      /* 한 코어 파일의 최대 크기 (바이트)*/
#ifdef  RLIMIT_RSS
    ssu_show(RLIMIT_RSS); /*메모리 RESIDENT SET SIZE 크기*/
#endif
#ifdef  RLIMIT_MEMLOCK
    ssu_show(RLIMIT_MEMLOCK); /*프로세스가 가질 수 있는 파일자물쇠 개수*/
#endif
#ifdef  RLIMIT_NPROC
    ssu_show(RLIMIT_NPROC); /*실제 사용자 ID 당 최대 Child Prodess 개수*/
#endif----
#ifdef  RLIMIT_OFILE
    ssu_show(RLIMIT_OFILE); /*프로세스가 열 수 있는 파일 최대 개수*/
#endif
#ifdef  RLIMIT_NOFILE
    ssu_show(RLIMIT_NOFILE); /*프로세스가 열 수 있는 파일 최대 개수 (위와 동일)*/
#endif
#ifdef  RLIMIT_AS
    ssu_show(RLIMIT_AS); /* 프로세스의 총 자유 메모리 개수*/
#endif
    

}

void ssu_show_limit(int rlim_type, char *rlim_name)
{
    struct rlimit rlim;
    char cur[11], max[11];

    getrlimit(rlim_type, &rlim);

    /* 해당 값이 LIMIT 값이면 리미트값임을 넣어주기*/
    if (rlim.rlim_cur == RLIM_INFINITY)
        strcpy(cur, "Infininty");
    else
        sprintf(cur, "%10ld", rlim.rlim_cur);
    

    if(rlim.rlim_max == RLIM_INFINITY)
        strcpy(max, "Infinity");
    else
        sprintf(max, "%10ld", rlim.rlim_max);
    printf("%15s : %10s %10s\n", rlim_name, cur, max);
}

/*    
    resource soft limit hard limit      

     RLIMIT_CPU :  Infininty   Infinity
   RLIMIT_FSIZE :  Infininty   Infinity
    RLIMIT_DATA :  Infininty   Infinity
   RLIMIT_STACK :    8388608   Infinity
    RLIMIT_CORE :          0   Infinity
     RLIMIT_RSS :  Infininty   Infinity
 RLIMIT_MEMLOCK :      65536      65536
   RLIMIT_NPROC :      50995      50995
   RLIMIT_OFILE :       4096       4096
  RLIMIT_NOFILE :       4096       4096
      RLIMIT_AS :  Infininty   Infinity
*/
