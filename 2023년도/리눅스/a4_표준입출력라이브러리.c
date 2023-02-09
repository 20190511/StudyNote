/* 0. Shell 등 Others*/
1. errno 는 <errno.h>에서 확인할 수 있다.

/* 1. fopen, freopen, fdopen : FILE *fp 구조체 변수를 만드는 함수
  */
#include <stdio.h>
FILE *fopen (const char *path, const char *mode);               // FILE *fp 를 return 해주는 함수.
FILE *fdopen (int fd, char *mode);                              // fd(파일 디스크립터) 를 이용해서 fp 를 return해주는 함수.
FILE *freopen (const char *path, const char *mode , FILE *fp);  // 이미 오픈한 파일(fp)을 다른 모드로 다시 fp2로 호출할 떄 사용.
  return 성공 시 파일 포인터(FILE *) 실패시 NULL -> errno설정
