/* 1. stat,fstat,lstat : 파일 정보를 가진 stat 구조체 생성. : 쉘로 치면 ls 명령어.
* restrict 키워드 : 특정 메모리 영역에 하나의 포인터만 접근할 수 있음

struct stat의 멤버변수 중 mode_t st_mode의 경우 -> 16비트 (unsigned int)로 구성되어있다.
  (4비트 :파일 종류, 3비트 : 유효사용자ID, 9비트 : 접근권한(USER, GROUP, OTHER, r/w/x)
*/
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int stat (const char *restrict path, struct stat *restrict buf);  // 경로 이용 파일의 stat 리턴
int fstat (int fd, struct stat *restrict buf);                    // 파일 디스크립터 이용 파일의 stat 리턴
int lstat (const char *restrict path, struct stat *restrict buf); // 심볼릭 링크 파일의 그 자체 stat 리턴
  return 성공시 0 실패시 -1 -> errno 설정
  
  
