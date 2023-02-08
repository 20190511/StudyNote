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
  return 이전의 파일 모드 생성 마스크.

/*5. chmod, fchmod : 적근 권한 변경
  mode 는 stat 의 st_mode 로 위와 동일.
  fchmod의 fd는 개방된 file desriptor 사용해야한다.
*/
#include <sys/stat.h>
int chmod (const char* path, mode_t mode);
int fchmod (int fd, mode_t mode);
  return 성공 시 0, 실패 시 -1 -> errno 생성;


/* 6. chowd, fchowd, lchown : 소유자 ID 와 그룹 ID 변경
  :사용자 식별 아이디 uid, gid를 변경하는 함수.
*/
#include <unistd.h>
int chown (const char *path, uid_t owner, gid_t group);           // uid,gid 
int fchown (int fd, uid_t owner, gid_t group);                    // 파일디스크립터를 이용한 uid,gid 변경
int lchown (const char *path, uid_t owner, gid_t group);          // Symbolic Link 파일 자체의 uid,gid 변경
  return 상공 시 0, 실패 시 -1 ->errno 생성;


/* 7. truncate, ftruncate : length(바이트) 만큼 파일을 자르는 함수
  파일의 크기 < length 을 경우, 파일의 크기가 늘어난다.
  파일의 크기 > length 의 경우, 파일의 크기가 length만큼 줄어든다.
    length는 off_t 타입으로 lseek 와 같은 long 타입이다.*/
#include <unistd.h>
#include <sys/types.h>
int truncate(const char *path, off_t length);                     // 파일 길이를 length(byte)만큼 자름
int ftruncate(int path, off_t length);                            // 파일 길이를 length(byte)만큼 자름 (fd 이용)
  return 성공 시 0, 실패 시 -1 -> errno 생성;   


/* 8. link, unlink : Hard link  (
  link(원래 파일, 연결할 파일);
  unlink(링크 끊을 파일);   //unlink를 하여
*/
#include <unistd.h>
int link (const char* exist_path, const char* new_path);          // 파일을 다른 파일명으로 i-node에 접근함 (다른 이름으로 동일 폴더 공유)
int unlink (const char* path);                                    // 해당 파일의 링크를 끊음 (링크 count가 0이면 해당 i-node는 free space)

