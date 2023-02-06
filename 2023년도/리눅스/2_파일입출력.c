
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
ssize_t read (int fd, void *buf, size_t nbytes); //ssize_t : long, size_t : unsigned int 
   return 성공 시 읽어들인 바이트수, 파일의 끝을 읽는 경우 : 0, 실패한 경우 : -1;

// write : nbytes 만큼 write를 요청함. (return값: ssize_t와 nbytes 는 다를 수 없음, 다르면 error가 난 것이다.)
#include <unistd.h>
ssize_t write (int fd, void* buf, size_t nbytes);
  return 성공 시 쓴 바이트 수, 실패한 경우 : -1;

    
//pread + pwrite : read와 write 연산을 원자적(atomic) 하게 수행함. :★ 요청한 바이트 수보다 적은 수가 return 되더라도 오류가 아님!
/* 원자적(atomic) 연산 이란 : 연산을 하는 도중에 가로챌 수 없고 + 파일 offset 위치가 변경되지 않는 연산*/
//lseek + read() 동시구현된 함수.
#include <unistd.h>
ssize_t pread (int fd, void *buf, size_t nbyes, off_t offset); //off_t : long, 
  return 성공 시 읽어들인 바이트 수, 파일의 끝을 있는 경우 : 0, 실패한 경우 : -1;
ssize_t pwrite (int fd, void *buf, size_t nbytes, off_t offset); //nbytes != return된 ssize_t 여도 오류가 아님!
  return 성공 시 기록한 바이트 수, 실패한 경우 : -1;
    
    
// lseek : 해당 파일디스크립터 의 파일 offset(위치) 변경.
/* 파일 크기보다 큰 offset을 설정할 수 있음. (그만큼 hole이 생긴다.)
   다른 fd가 lseek을 호출해도 개별적인 offset을 가짐. (같은 fd엔 같은 offset!)
*/
#include <sys/type.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);  //off_t 는 long 타입을 의미함.
  return 성공시 바뀐 위치 offset (long타입), 실패시 -1; 



