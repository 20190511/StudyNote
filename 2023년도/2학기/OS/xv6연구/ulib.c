#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

/**
 * {문자열 복사함수}
 * *s에 *t를 복사하는데 중간에 \0이 만나면 그만둠
 * 즉, s<--t 를 복사하는 함수.
*/
char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

/**
 * {문자열 비교 함수 : }
 * strcmp(A,B) 라고 했을 때 A의 요소 *a 와 B의 요소 *b 를 비교함
 * a 와 b가 같지 않을때 까지 A++, B++ 를 하고 마지막에 A-B 를 함.
 * 즉, return 값이 음수라면 A가 더 작은것,
 * 즉, return 값이 0이라면 A==B
 * 즉, return값이 양수라면 A가 더큰 것.
*/
int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

/**
 * {문자열 크기를 알아내는 함수.}
*/
uint
strlen(const char *s)
{
  int n;

  //s[n] 이 0 (==\0) 이 아닐때 까지 탐색, 즉 문자열 크기 알아냄.
  for(n = 0; s[n]; n++)
    ;
  return n;
}

/**
 * {memset 함수, stosb 시스템 콜을 사용해서 c로 설정 memset(void* dst, int c, uint n)}
 * dst 를 n 만큼 정수형 c로 설정.
*/
void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

/**
 * {strchr(char* s, char c), s에서 앞부분부터 문자 c를 찾은 위치.}
 * 만약 못찾았으면 0 리턴.
 * 
*/
char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

//문자열 입력 함수.
/** 
 * 표준 입력 (0) 으로부터 한 줄씩 받아온다. (개행문자포함.)
 *  --> 만약 파일입출력을 하고싶으면 read(0, ~~) 부분을 수정하자.
 * 성공시 buf 리턴.
*/
char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

//평범한 stat 함수, stat 구조체를 받아온다.
/**
 * fstat(fd, struct stat* st) 시스템 콜을 이용해서 구조체를 받아온다.
 *  fstat 구조체는 파일 이름 대신 fd(파일디스크립터) 를 받나보다.
 * 성공시 0, 실패시 -1
*/
int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

/**
 * {문자열을 --> 정수화}
 * 문자열이 만약 1234CH123 이면 1234 까지만 받아옴.
*/
int
atoi(const char *s)
{
  int n;

  n = 0;
  //문자열 부분 중 정수부분만 캐치
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

/**
 * {memmove(A,B,size) : B --> A 로 복사, (B를 A로 size 만큼 복사)}
*/
void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  // dst 에 src를 size만큼 복사.
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}
