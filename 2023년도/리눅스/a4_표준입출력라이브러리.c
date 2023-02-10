/* 0. Shell 등 Others*/
1. errno 는 <errno.h>에서 확인할 수 있다.;
2. tty      : 현재 '표준 입력'에 접속된 '터미널 장치 이름'을 출력하는 명령어 <- /dev/pts/6 을 fp로 설정하면 명령어창(terminal)에 출력됨;
  ex>;
  junhyeong@DESKTOP-UPFPK8Q:~/go2$ tty;
  /dev/pts/6;
2-2. 터미널이란? :Linux 서버에 SSH로 연결할 때 '로컬 컴퓨터에서 실행'하고 '명령을 입력하는 프로그램';
2-3. int isatty(int fd) 해당 파일 디스크립터가 terminal 인지 확인해주는 함수 :맞으면-> 0(True), 틀리면 1(False);
3. system(char *str)  : 입력받은 문자열을 system 쉘 입력으로 바꿔서 실행해줌;
4. ★ 디버그모드에서 strerror(errno) 해주면 애러넘버를 받을 수 있다.
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


/* 4. fflush : 버퍼의 내용을 해당 파일에 즉시 저장 + 호출 후에도 해당 fp는 계속 Open된 상태로 유지)*/
#include <stdio.h>
int fflush (FILE *fp);
  return 성공시 0, 실패시 EOF(-1) -> errno설정;

/* 5.getc, fgetc, getchar : 한 번에 하나의 문자를 입력받는 함수
 * 위의 세 함수는 에러나 파일의 끝 모두 EOF(-1) 이므로 feof() 나 ferror() 와 함께 사용!
 * getchar(void) == fgetc(stdin)
 */
#include <stdio.h>
int getc (FILE *fp);             // 매크로로 구현된 fp의 한 글자를 입력받는 함수
int fgetc(FILE *fp);             // 함수로 구현된 fp의 한 글자를 입력받는 함수
int getchar(void);               // stdin으로부터 한 글자를 입려받는 함수: char->int로 변환.
  return 성공시 0, 실패 or 파일의 끝일때 EOF(-1);


/* 6. ferror, feof, clearerr : 앞의 getc,fgetc 의 EOF가 에러인지 파일의 끝인지 확인 시켜주는 함수 
 * ferror(fp)   <- 앞의 EOF가 입출력 에러인지 알려주는 함수
 * feof(fp)     <- 앞의 EOF가 파일의 끝에 도달했는지 알려주는 함수
 * clearerr(fp) <- error 플래그를 없애주는 함수
 */
#include <stdio.h>
int ferror (FILE *fp);           // EOF 가 입출력 에러면 0, 아니면 0이 아닌값
  return EOF 가 입출력 에러면 0, 아니면 0이 아닌값;
int feof (FILE *fp);             // EOF 가 파일의 fp._flag가 EOF 플래그가 설정되어 있으면 0, 아니면 0이 아닌수
  return  EOF 가 파일의 fp._flag가 EOF 플래그가 설정되어 있으면 0, 아니면 0;
void clearerr (FILE *fp);        // 에러 플래그들을 모두 초기화시켜줌 <- 에러 뜨고 clearerr 안해주면 에러값들이 초기화되지 않아 계속 남아있음

/* 7. ungetc(char x,fp) : 해당 fp stream 에다가 'x'를 하나 넣어주는 함수 
 * 예를들어 fp stream이 처음에 123+456 에서 쭉쭉 가서 456 이 되었을 때
 * ungetc('k',fp); 를 해주면 stream이 k456이 되고 이상태에서 getc(fp);를 해주면 456으로 stream이 바뀜 
 */
#include <stdio.h>
int ungetc(int c, FILE *fp);
  return 성공시 c, 실패시 EOF;

/*8. putc, fputc, putchar(void) : 한 번에 하나의 문자를 출력하는 함수 */
#include <stdio.h>
int getc (int c, FILE *fp);             // fp에 c를 출력하는 함수 (함수)
int fgetc(int c, FILE *fp);             // fp에 c를 출력하는 함수 (매크로)
int getchar(int c);                     // c를 모니터에 출력해줌
  return 성공시 0, 실패 or 파일의 끝일때 EOF(-1);

/* 9. fgets, gets : 문자열을 입력받는 함수
  fgets 시 fp로 부터 ★(개행문자('\n' | 파일의끝 | 버퍼사이즈 중 작은값) 만큼 읽어들인 값을 buf에 저장하는 것.
      buf size가 n이라 size=n으로 데이터를 넘겨주면 n-1개까지 읽고 마지막은 항상 NULL 처리된다. 
        (여기서 마지막이란 buf 의 마지막값 인덱스와 읽어들인 값이 k(k<n) 이라면 k+1가 '\0' 으로 바뀜을 의미한다.
  그리고 fgets() 재호출 시 나머지 값들을 읽어온다.
  gets() 시 표준 입력을 받을 떄, '\n'을 NULL로 대체해서 읽는 함수이다..*/
#include <stdio.h>
char *fgets(char *buf, int n, FILE *fp);  // fp에 buf를 n만큼 입력받음
char *gets (char *buf);                   // stdin으로 buf에 입력받음


/* 10. fputs, puts : 문자열을 출력받는 함수
  fputs() 의 경우 : FILE *fp 에 인자 str 를 출력하는데 ★ NULL과 개행 문자 '\n' 는 출력하지 않는다.
        -> 문자처리된 "\n"은 인식은 되지만, "example\n" 이라고 저장되면 "example" 까지만 들어가고 \n은 개행처리를 해주는 식.
  puts() 의 경우  : FILE *fp 에 인자 str 를 출력하는데 ★ NULL 를 개행문자 '\n' 로 바꿔서 출력한다.
  */
#include <stdio.h>
int fputs (const char* str, FILE *fp);
int puts (const char *str);
  return 성공시 0이 아닌값, 실패 시 -1 (EOF);

/* 11. fread, fwrite : 바이너리 파일 입출력 : 구조체를 한꺼번에 저장할 때 매우 유용 (void*) ptr 타입으로 받기 떄문 
 * fread (&구조체변수, sizeof(구조체변수), 1, fp); //n_obj는 구조체나 객체의 개수 처럼 사용
 */
#include <stdio.h>
size_t fread (void *ptr, size_t size, size_t n_obj, FILE *fp);  //n_obj : 구조체나 객체의 개수. 바이너리 파일을 읽는데 사용
size_t fwrite (void *ptr, size_t size, FILE *fp);               // 바이너리 파일을 쓰는데 사용.
  return 성공적으로 읽거나 쓴 객체의 개수 (obj개수);

/* 12. ftell, fseek, rewind : 파일 오프셋 위치 정보 / 수정 / 처음으로 되돌림 함수*
 * ftell : 파일의 처음(SEEK_SET)으로부터 몇 '바이트' 떨어져 있는가? 알려주는 함수. 해당 바이트값 return (이 값은 fseek( ,long offset, )에 쓰임
 * fseek : ★ fp 가 open 되어있을 떄 쓸 수 있고, whence + offset 만큼 파일 오프셋을 수정함.
 *           주의할 점은 ★★ lseek과 다르게 return 값이 파일 오프셋이 아니라 0을 return함
 * rewind : 파일의 시작점으로 파일 오프셋을 되돌림. == fseek(fp, 0, SEEK_SET); 
 */
#include <stdio.h>
long ftell (FILE *fp);                          // 파일 오프셋이 파일시작점(0)으로부터 몇 바이트 떨어져있는가?
  return 시작점(0)으로부터 몇 바이트 떨어져있는지 long으로 반환;
int fseek(FILE *fp, long offset, int whence);   // 파일 오프셋 수정 (whence+offset)
  return ★ 성공 시 0, 실패 시 0이 아닌값;
void rewind(FILE *fp);                          // 파일 오프셋을 SEEK_SET(0) 으로 되돌림.

/* 13. ftello, fseeko :  파일 오프셋 위치 정보 / 수정 (타입 확장버젼) 
 * 앞의 ftell과 fseek하고 다 똑같은데 long -> off_t 로 바뀔 뿐이다. 이는 long보다 큰 파일 offset이 주어졌을 때
 * off_t 타입으로 재지정함으로서 파일 오프셋을 설정할 수 있게 하기위함
 */
#include <stdio.h>
off_t ftello (FILE *fp);                          // 파일 오프셋이 파일시작점(0)으로부터 몇 바이트 떨어져있는가?
  return 시작점(0)으로부터 몇 바이트 떨어져있는지 long으로 반환;
int fseeko (FILE *fp, off_t offset, int whence);   // 파일 오프셋 수정 (whence+offset)
  return ★ 성공 시 0, 실패 시 0이 아닌값;

/* 14. fgetpos, fsetpos : 파일 오프셋 현재 위치를 얻어 pos에 저장한 후 .. 오프셋 작업 후 .. 다시 현재위치로 되돌리는 함수 
 * 현재 스트림의 오프셋 위치를 기억하기 위한 함수
 */
#include <stdio.h>
int fgetpos (FILE *fp, fpos_t pos);               // pos에 현재 파일 offset 저장
  return 성공 시 0, 실패시 1;
int fsetpos (FILE *fp, fpos_t pos);               // pos에 들어있는 offset으로 되돌림.  ㅂ1
  return 성공시 0, 실패 시 0이 아닌값 -> errno 설정;

/* 15. printf, fprintf, sprintf, snprintf : 가변적인 서식문자 출력함수 
 * %[플리그][필드 넓이][정밀도][길이수정자][변환 형식]
 */
#include <stdio.h>
int printf (const char* format, ...);                         // 표준출력(stdout) 으로 서식문자 출력
int fprintf (FILE *fp, consth char* format, ...);             // fp 에 서식문자 출력
  return 성공시 출력된 문자개수, 에러시 음의 값;
int sprintf (char *buf, const char *format, ...);             // buf 에 서식문자 저장(출력)
int snprintf (char *buf, size_t n, const char *format, ...);  // buf 에 n바이트만큼 서식문자 저장 (출력) : 더안전함(buffer Overflow 방지)
  return 성공시 배열에 저장된 문자개수, 에러시 음의값;

/* 16. scanf, fscanf, sscnf : 서식문자 입력 함수
 * 서식문자 입력 % 와 하나라도 부합하지 않으면 중지되거 나머지 부분은 읽히지 않는다.
 * %[*][필드 넓이][길이 수정자][변환형식]
 * char buffer[256]="name:홍길동 num:12 age:20"; ┐
 * sscanf(buffer,"name:%s num:%d age:%d",name,&num,&age);
 */
#include <stdio.h>
int scanf(const char *format, ...);                           // 표준 입력을 받아 해당 변수값들에 저장.
int fscanf (FILE *fp, const char *format, ...);               // 서식문자의 값을
int sscanf (char *buf, const char *format, ...);              // 서식문자 값을 buf에 저장
  return 성공시 입력항목의 개수, 어떠한 변화도 일어나기 전에 입력이 끝나면 EOF, 읽기 에러발생 시 EOF 발생 후 ->errno 설정;

/* 17. fileno : 오픈된 fp 로부터 fd(파일디스크립터) 를 얻는 함수. */
#include <stdio.h>
int fildno (FILE *fp);                                         // FILE *fp -> int fd 파일디스크립터 추출
  return 스트림에 연관된 파일디스크립터 실패시 -1 -> errno설정;

/* 18~19. tmpnam, tmpfile, trempnam : 임시파일 생성 
 * tmpnam : 임시파일명을 받는다.(NULL이면 만들기만하고, ptr을 넣어주면 해당 값에 문자열포인터를 할당해준다.)
 *      프로그램이 종료되도 파일이 삭제되지 않으므로 주의할 것. (rename이나 unlink를 해줄것)
 *       만드는 도중 remove를 시도해보았으나 'NO Such File or Directory' 발생 + 실제로 찾아보니 안보이는 문제발생.
 * tmpfile : 임시파일을 무자위로 만들어 사용한 뒤 프로그램이 종료되면 해당 파일은 지워진다.
 * tempnam : 디렉토리(directory), 5자 이내의 접두어(prefix) 를 설정하여 임시파일명을 만들 수 있다
               :  파일 명을 만드는거지 파일을 만들어 주는 것은 아니다. 만들려면 해당 이름으로 open으로 해줘야한다.
 */
#include <stdio.h>
#include <stdlib.h>
char* tmpnam (char *ptr);                                   // 문자열 ptr에다가 임시네임을 만들어서 할당해줌.
  return 성공시 고유한 임시파일의 경로 이름을 가리키는 포인터, 실패시 NULL;
char* tempnam (const char *dictionary, const char *prefix)  // dictionary 경로와 5자 이내의 prefix를 조합하여 임시명을 만들어줌.
  return 성공시 고유한 임시파일의 경로 이름을 가리키는 포인터, 실패시 NULL->errno설정;
FILE* tmpfile (void);                                       // stdin 내부에서 임시파일을 만들어줌.
  return 성공시 파일 포인터, 실패시 NULL -> errno 

