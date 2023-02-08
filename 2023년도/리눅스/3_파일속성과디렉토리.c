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
  
  
/* 2. stat 구조체 중 st_mode 를 이용한 타입 추출  [st_mode의 상위 4비트는 파일 종류를 나타냄] */
ex) S_ISDIR(stat.st_mode); -> dictionary면 true, 아니면 false;

/* 3. Access : 파일 접근 권한 검사 (접근할 수 있는가 ? ) 
  mode <- R_OK , W_OK , X_OK , F_OK  = {읽기권한, 쓰기권한, 실행권한 , 파일 존재여부}
   ex) access("linux.txt", R_OK);
  */
#include <unistd.h>
int access (const char * path, int mode);                         // 파일 접근 가능 여부
  return 성공시 0, 실패 시 -1 ->errno 설정;
  
/* 4. umask : 1로 설정 시 해당 사용자의 해당권한 금지 
    cmask 는 mode_t 의 S_IROTH 등과 같이 mode_t의 플래그 사용
      ex) ummask( S_IROTH); <- Other 사용자의 Read(읽기) 권한 금지.*/
#include <sys/types.h>
#include <sys/stat.h>
mode_t umask(mode_t cmask);
