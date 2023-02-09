/* 0. Shell 등 Others*/
1. errno 는 <errno.h>에서 확인할 수 있다.;
2. tty      : 현재 '표준 입력'에 접속된 '터미널 장치 이름'을 출력하는 명령어 <- /dev/pts/6 을 fp로 설정하면 명령어창(terminal)에 출력됨
  ex>
  junhyeong@DESKTOP-UPFPK8Q:~/go2$ tty
  /dev/pts/6
2-2. 터미널이란? :Linux 서버에 SSH로 연결할 때 '로컬 컴퓨터에서 실행'하고 '명령을 입력하는 프로그램';




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
int fclose (FILE *fp);                          // 해당 fp 를 닫는 함수 <- ffulsh() 를 호출해서 비워줘야함. (수동)
 return 성공시 0, 실패시 EOF(-1) -> errno설정;

#define _GNU_SOURCE
#include <stdio.h>
int fcloseall (void);                           // 모든 stream 을 닫고 buffer를 fflush 해버리는 함수.
 return 성공시 0, 실패시 EOF(-1) -> errno설정;

/* 3. setbuf, setvbuf : '버퍼'를 설정하는 함수
 * 버퍼링이란?  : 읽기/쓰기 시스템 호출 양을 줄이기 위해서 고안된 방법 : 버퍼값에 데이터를 넣어넣고 일정이상 차면 콜을 하자.
 * 버퍼의 종류  : _IOFBF (Fully-Buffered), _IOLBF (lined-Buffered), _OFNBF (Non-Buffered)
 * setbuf() 사용법 : buf=NULL <- Non-Buffered, buf != NULL <- buf[BUFSIZ] 만큼의 버퍼가 됨
 *    -> But, fp=터미널(ex)stdout,등등..) lined-Buffered가 됩니다.
 * setvbuf() 사용법 : mode와 buf 설정이 가능하다. 위의 mode 방식으로 사용 가능
 * ★ 중간에 버퍼 값을 바꾸면 -> 전의 값을 fflush() 해버리고 새로운 buf값 설정
 */
#include <stdio.h>
void setbuf (FILE *fp, char *buf);                        // buf가 NULL이면 Non-Bufferd, 아니면 buf[BUFSIZ] 버퍼설정
int setvbuf (FILE *fp, char *buf, int mode, size_t size); // buf가 NULL이면 mode따라 버퍼설정, 아니면 해당 매개변수에 따른 버퍼설정
  return 성공시 0, 실패시 0이 아닌 다른값 ★; 


/* 4. 
