
// 1. open + creat
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int open(const char* pathname, int oflag, mode_t mode); // oflag 는 fcntl 상수 이용.
  return 성공시 fd, 실패시 -1;
int creat(const char* pathname, mode_t mode); //접근은 write가 있어야함 -> 읽을 때는 쓰기/읽기모드로 다시 호출해야함.
  return 성공시 fd, 실패시 -1;


// close
#include <unistd.h>
int close(int fd);

// read : 오픈된 파일에서 데이터를 읽는데 사용. buf : 읽어들인 데이터를 넣을 buffer, nbytes : 읽어들일 바이트 수
/* 파일의 크기가 5가 남았는데 10을 읽으려고하면 5만큼 읽고 return -> 계속 읽으려고하면 파일의 끝에 도달하여 0 return*/
#include <unistd.h>
int read (int fd, void *buf, size_t nbytes); //size_t : unsigned int 
   return 성공 시 읽어들인 바이트수, 파일의 끝을 읽는 경우 : 0, 실패한 경우 : -1




// lseek : 해당 파일디스크립터 의 파일 offset(위치) 변경.
/* 파일 크기보다 큰 offset을 설정할 수 있음. (그만큼 hole이 생긴다.) */
#include <sys/type.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);  //off_t 는 long 타입을 의미함.
  return 성공시 바뀐 위치 offset (long타입), 실패시 -1; 



