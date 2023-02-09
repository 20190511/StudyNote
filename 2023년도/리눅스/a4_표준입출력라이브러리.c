/* 0. Shell 등 Others*/
1. errno 는 <errno.h>에서 확인할 수 있다.

/* 1. fopen, freopen, fdopen : FILE *fp 구조체 변수를 만드는 함수
  
  freopen 함수같은 경우 -> 이미 열려있는 fp 를 close해서 닫고 path의 fp로 다시 오픈하는 함수이다.
   마치 저수준IO의 dup2와 비슷하게 할 수 있는데 close를 해준다고 생각하면 된다 (dup2:Copy, freopen:move);*/
#include <stdio.h>
FILE *fopen (const char *path, const char *mode);               // FILE *fp 를 return 해주는 함수.
FILE *fdopen (int fd, char *mode);                              // fd(파일 디스크립터) 를 이용해서 fp 를 return해주는 함수.
FILE *freopen (const char *path, const char *mode , FILE *fp);  // 이미 오픈한 파일(fp)을 다른 모드로 다시 fp2로 호출할 떄 사용.
  return 성공 시 파일 포인터(FILE *) 실패시 NULL -> errno설정;

/* 2. fclose, fcloseall : 오픈된 파일 스트림(fp) 을 닫는 함수
  닫으려는 fp가 출력용인 경우 fflush(fp) 를 써서 입출력 버퍼의 데이터를 모두 써야한다(데이터 저장을 위해)
  fcloseall() 의 경우에는 모든 스트림을 닫고 닫기 전의 모든 남은 내용을 스트림에 쓰고, 0을 return 한다 */
#include <stdio.h>
int fclose (FILE *fp);
 return 성공시 0, 실패시 EOF(-1) -> errno설정;

#define _GNU_SOURCE
#include <stdio.h>
int fcloseall (void);
 return 성공시 0, 실패시 EOF(-1) -> errno설정;

