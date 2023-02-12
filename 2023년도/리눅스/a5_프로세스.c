/* 0. Shell 명령어 및 C언어 문법 */
 1. find                     : 현재 할당된 메모리 값들을 볼 수 있음;
 2. char *envp[] 를 함수 매개변수로 포인터 자체로 받고 싶은 경우 (복사 연산 없이);
      void XXX (char ***ptr); 로 선언하면 된다. <- 실 사용시 XXX (&envp);
 3. #define AA(X)  AA(#X,X)  : 매크로 선언 때 #를 붙이면 해당 변수를 문자열화 해서 받을 수 있다;
 4. file descriptor 의 경우 표준 입/출력/에러는 매크로로도 설정되어 있다;
       return STDIN_FILENO : 0, STDIN_FILENO : 1, STDERR_FILENO : 2;
 5. 자식프로세스 status는 앞부분(하위) 8비트 + 상위 8비트 (??값) 으로 전달되는데 WEXITEDSTATUS(status)로 호출하기 싫다면;
       status >> 8; 로 호출해도된다;
 6. int* 값을 NULL 로 전달하고 싶으면 (int*)NULL 혹은 (int*)0 으로 전달해도된다;
 7. echo    "전달 문자열"     : 현재 전달할 문자열 echo 출력

/* 1. exit, _Exit, _exit : 프로세스 종료 함수 
 * exit는 표준입출력 라이브러리, 종료처리부들 등을 정리하고 종료 -> 부모에게 status 전달
 * _Exit, _exit 는 위의 부분들을 정리하지 않고 즉시 종료.
 * status는 8비트까지만 사용하므로 : 0~255 까지의 정수를 부모프로세스에게 넘겨줄 수 있음
 * 실제과정은 exit() 호출 후 파일 스트림을 닫고 표준입출력을 종료한 후 _exit()를 호출한다 -> 이후 운영체제가 fd를 닫고 메모리할당을 해제한다.
 */
#include <stdio.h>
void exit (int status);                 // std Library, 종료처리부들 등을 정리하고 -> _exit()호출
void _Exit (int status);                // 정리하지 않고 즉시종료 -> 부모프로세스에게 status 전달

#include <unistd.h>
void _exit (int status);                // 정리하지 않고 즉시종료 -> 부모프로세스에게 status 전달


/* 2. atexit, on_exit() : 종료처리 과정 중 호출될 함수 
 * etexit : void(*func)(void) 함수가 '최신에 등록된 순서' 로 호출됨.
 *          중요한 점은 exit()는 호출할 시 무한재귀호출이 일어나므로, _exit()를 호출할 것.
 * on_exit() : void(*func2)(int, void*) 함수를 호출 할 수 있고
 *              arg인자로 종료과정에 메모리포인터를 전달해줄 수 있으나, 해당값이 유효해야함
 */
#include <stdlib.h>
int atexit ( void(*func)(void));                    // 종료처리과정 중 함수호출 가능 (void타입, void매개변수)
int on_exit( void(*func)(int, void*), void* arg);   // 종료처리과정 중 함수호출 가능 (void타입, int,void*매개변수), 메모리포인터  
  return 성공시 0, 실패시 0이외의 값;


/* 3. main(int argc, char* argv[]) 생략*/
/* 4. malloc, calloc, realloc, free : 메모리 할당 함수 
 *    LINUX에서 메모리는 자유리스트(free list)를 통해 적절히 포인터를 조정해서 메모리 블록을 가져온다.
 *    만약, 메모리 풀에서 원하는 블록이 없을 경우에는 프로세스의 heap(힙) 크기를 조정할 수 있는 sbrk() 시스템콜을 하여
 *    시스템으로부터 더 많은 메모리를 가져올 수 있다.
 *    ★ realloc의 size는 새로 할당할 전체크기의 사이즈로 설정해야한다. (늘어난 크기가 아닌, 늘어난 전체크기를 넣어야한다.)
 *
 *  <메모리 할당 시 주의할 점> 
 * A. 이미 할당해제한 메모리 블록 (자유 메모리 풀에 있는 메모리블록) 을 다시 free를 하는 경우 
 *     혹은 할당받지않은 메모리에 대해 free()를 하는 경우
 *     메모리 블록 헤더를 손상시켜 치명적인 결과를 가져올 수 있으므로 주의해야한다.
 * B. free를 제대로 하지않으면 메모리들이 프로세스 중 지속적으로 누적이되어 메모리 누출 (Memory Leak) 이 발생하여 
 *      프로세스 실행속도에 영향을 미친다.
 */
#include <stdlib.h> 
void* malloc (size_t size);                         // size 공간만큼 자유메모리(free memory) 에서 메모리공간을 할당받음
void* realloc (void* ptr, size_t newsize);          // ptr 메모리공간의 크기를 newsize로 다시 할당받음 (원본데이터는 변하지않음)
void* calloc (size_t obj_size, size_t count);       // 개당 메모리블럭 사이즈(obj_size)를 count개수만큼 메모리공간에 할당받고 이를 다 0으로 채움.
  return 성공시 할당된 메모리를 가리키는 포인터, 실패시 NULL;
void free(void* ptr)l                               // 메모리 할당해제
  

/* 5. getenv, putenv, setenv, unsetenv : 환경변수 설정*/
#include <stdlib.h>
char *getenv (const char* name);                                    // name에 해당하는 환경변수 값을 가져옴.
  return 성공시 name으로 주어진 이름에 해당하는 환경변수 포인터 반환, 실패시 NULL;
int putenv (char* str);                                             // str = "name=value" 값으로 환경변수 설정
  return 성공시 0, 실패시 0이아닌값 -> errno 설정;
int setenv (const char *name, const char *value, int rewrite);      // name="name", value="value" 값으로 환경변수 설정, rewrite는 Overwrite 여부 확인
  return 성공시 0, 실패시 -1 -> errno 설정;                     
int unsetenv (const char *name);                                    // name에 해당하는 환경변수 삭제.
  return 성공시 0, 실패시 (해당 이름이 없는경우) -1 -> errno 설정;


/* 6. setjmp, longjmp : 다른 함수로 jump 하는 기능 (goto 확장판)
 *  goto문은 다른 함수로의 jump가 불가능하지만, setjmp, longjmp를 이용하면 다른 함수로의 jump가 가능해진다.
 *  원리는 setjmp(jmp_buf env); 를 할 때 해당 위치에서의 레지스터, 스택 환경 등을 env 구조체에 저장해놓고 
 *  longjmp를 호출해 그 위치로 돌아가는 것이다.
 *  ++ longjmp(jmp_buf env, int val) 에서 val은 setjmp의 return값이 되어 setjmp 을 호출했을 때 어느 longjmp에서 분기가 되었는지 판단가능하다.
 *  + 컴파일러 최적화(gcc -o 실행파일 소스파일 -0) 을 해주면 setjmp를 할 때 해당위치까지 '지역변수' 까지도 모두 저장해서 분기가 가능하다.  
 *  + ★ jmp_buf는 '전역변수' 로 선언해야한다!
 */
#include <stdjmp.h>
int setjmp (jmp_buf env);                          // setjmp 위치까지 스택/레지스터 환경상태를 env에 저장함 (-0 옵션 시 지역변수도 저장) 
 return 자기호출시 0, longjmp에 의한 호출 시 longjmp의 val값;
void longjmp (jmp_buf env, int val);               // env 위치에 저장된 장소로 goto


/* 7. getrlimit, setrlimit : 프로세스의 적용된 자원의 한계값 을 탐색/설정 하는 함수
 *  자원 한계에는 약한한계(Soft Limit) 와 강한한계 (Hard Limit) 가 있다
 *  Soft-Limit는 프로세스에 시그널까지로 끝낼 수 있지만, Hard-Limit 를 초과해서 자원을 사용할 수는 없다. (루트만 Hard-limit 를 올릴 수 있다.)
 *  일반 사용자는 0 ~ rlim_max 사이의 rlim_cur 값을 조정할 수 있을 뿐이다.
 *  C언어에서는 이 Soft/Hard limit 를 정의한 구조체를 struct rlimit 로 제공하고 있다.
 *        <struct rlimit>
 *           rlim_t rlim_cur;   //soft limit (0~rlim_max 사이의값)
 *           rlim_t rlim_max;   //hard limit
 */
#include <sys/resoucre.h>
#include <sys/times.h>
//resource 는 sys/resource에 매크로로 설정되어있음
int getrlimit (int resource, struct rlimit *rlptr);                  // 해당 resource에 해당하는 rlimit rlptr 구조체 설정
int setrlimit (int resource, struct const rlimit *rlptr);            // 해당 resource에 해당하는 rlimit rpltr 구조체로 재설정
 return 성공시 0,  실패시 -1 -> errno 설정;


/* 8. getpid, getppid, getuid, geteuid, getgid, getegid : PID/ID (프로세스ID/ID)를 얻어오는 함수 
 * 프로세스는 각 프로세스마다 자신의 고유의 ID가 존재한다. 이는 고유의 유일한 속성이기 때문이다.
 * PID=0 프로세스 : 디스크에 존재하지 않고 메모리에 존재하는 프로세스로 (fork() 호출 시 자식 프로세스는 자신의 PID가 0으로 반환된다)
 * ex) init 프로세스는 PID = 1 이다 (UNIX/LINUX 부팅 시 가장먼저 실행되는 프로세스)  
 *
 * pid_t 는 32비트 정수값 = int 이다.
 * getpid   : 호출한 프로세스 PID
 * getppid  : 호출한 부모 프로세스 PID
 * getuid   : 호출한 실제 사용자 ID
 * geteuid  : 호출한 유효 사용자 ID
 * getgid   : 호출한 프로세스의 그룹 ID
 * getegid  : 호출한 프로세스의 유효 그룹 ID
 */

#include <unistd.h>
#include <sys/types.h>
pid_t getpid (void);            // 호출한 프로세스 PID
pid_t getppid (void);           // 호출한 부모 프로세스 PID
pid_t getuid (void);            // 호출한 실제 사용자 ID
pid_t geteuid (void);           // 호출한 유효 사용자 ID
pid_t getgid (void);            // 호출한 프로세스의 그룹 ID
pid_t getegid (void);           // 호출한 프로세스의 유효 그룹 ID
 return 성공 시 PID;

/* 9. fork : 자식 프로세스 생성 (새 프로세스 생성 -> 각 프로세스가 독립적으로 실행됨
 *        자식은 fork() 리턴값이 0, 부모는 자식의 PID (Process ID) 가 리턴된다.
 *   보통 fork()가 일어난 후 exec 계열 함수들이 자주호출되므로 메모리 측면의 절약을 위해
 *   부모<-> 자식 프로세스 간 메모리공간을 공유하여 사용하며 (Read-Only) 있다가 
 *   부모나 자식 중 어느 하나라도 메모리 공간에 접근하여 영역 수정 시 메모리 공간이 '복사'된다.
 *   여기서 '복사' 란 일반적으로 부모의 자료구조,힙,스택이 복사되며 서로의 tms나 PID,PPID 등은 복사되지 않는다.
 *   텍스트 구역은 '공유' 되는 공간이라 부모와 자식이 공유해서 사용된다.
 */
#include <unistd.h>
pid_t fork(void);               // 자식 프로세스 생성.
  return 자식프로세스인 경우 : 0 ; 부모프로세스인 경우 : 자식프로세스 PID;
         에러 시 -1 -> errno 설정;



/* 10. vfork : 자식 프로세스를 부모프로세스와 공유된 메모리 공간을 활용하여 생성 
 *             정확하게는, 자식 프로세스가 exec 계열 함수를 호출하여 메모리 공간을 수정하지 않는다는 전제하에 만듬 함수이다.
 *             실행방식은 : 자식 프로세스가 실행되고 exit() 호출 or exec() 계열 함수를 사용한 후 종료된 이후 부모 프로세스를 실행한다.
 *             ★ 이때 중요한 점은, 자식 프로세스는 종료할 때 exit()를 쓰면 안되고 _exit()를 써야한다! 
 *                -> 자식프로세스가 종료하면서 공유된 입출력 스트림을 닫아버리기 때문에 정리를 하지않고 종료하는 _exit()를 사용해야하는 것.
 *             ++ 자식과 부모프로세스가 공유된 메모리 공간을 쓰므로, 자식->부모 간 메모리복사는 일어나지 않는다.
 */
#include <unistd.h>
#include <sys/types.h>
pit_d vfork(void);
  return 자식프로세스인 경우 : 0 ; 부모프로세스인 경우 : 자식프로세스 PID; 
         에러 시 -1 -> errno 설정;


/* 11. wait, waitpid : 자식 프로세스들의 종료상태를 기다렸다가 자식 프로세스의 종료 status를 받아오는 함수.
 *     자식프로세스가 종료되면 자식프로세스의 status가 하위(앞의 비트) 8비트로 표현됨 (0~255까지 표현가능)
 *     종료상태를 알고싶으면 WIFEXITEDSTATUS(status) 를 넣으면 해당 값을 알 수 있음.
 *     -wait() 의 경우 '모든' 자식 프로세스가 종료해야 부모 프로세스 실행.
 *     -waitpid의 options 값은 상수(비트) 로 지정되어 있으며, | 연산자를 넣어 계산할 수 있음
 *      ++ int *statloc 값을 받기 싫으면 (int*)NULL 혹은 (int*)0 으로 넘겨주면된다.
 *     <waitpid option 상수들>
 *     a. WCONTINUED   : 자식 프로세스가 중지되었다가 SIGCONT 시그널로 재게되어도 return
 *     b. WNOHANG      : 자식 프로세스가 종료되지 않아도 즉시 종료 후 0 리턴 (자식프로세스가 종료되면 자식프로세스 pid 리턴)
 *     c. WUNTRACED    : 자식 프로세스가 종료 + '중지(멈춤)' 되어도 그 상태 return
 */
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait (int *statloc);                                  // 자식프로세스가 종료할 때까지 멈추었다가 부모 재게, statloc에 자식프로세스 하위비트값전달.
pid_t waitpid (pid_t pid, int* statloc, int options);       // 위의 option에 따라 해당 pid의 자식 프로세스 처리.
 return 성공시 자식프로세스ID ,에러시 -1;
 return (waitpid의 경우 options=WNOHANGS && 자식 프로세스가 종료되지 않으면) 0;

/* 12. waitid : waitpid와 비슷하나, 아래의 차이점 존재
 * idtype_t idtype, id_t id : idtype_t  enum {P_PID=0,P_PGID=1,P_ALL=2} 으로 전달 .
 *                            해당 enum값은 id_t (unsigned int 타입) id 로 전달 가능
 * siginfo_t *info_pointer  : 자식 프로세스의 시그널 정보를 상세하게 저장해줌.
 * int options              : 위의 waitpid option과 동일하게 기능 (몇 기능 추가)
 */
#include <sys/types.h>
#include <sys/wait.h>
int waitid (idtype_t idtype, id_t id, siginfo_t *info_pointer, int options); // 해당 idtype에 따라 wait기능 함수 작동
 return waitpid와 동일하게 작동;


/* 13. wait3, wait4 : wait() + 자식 프로세스 자원 사용량 정보를 얻는 함수
 *      wait3 에서 pid를 지정할 수 있으면 wait4 이다
 *      프로세스 사용정보는 rusage 구조체를 이용해 전달, 해당 구조체는 CPU 시간, 시스템 CPU 시간, page fault 횟수 등.. 저장  (resource usage)
 */
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
pid_t wait3 (int *statloc, int options, struct rusage *resage);             // 위에 설명 wait + 자식 프로세스 리소스 사용량 정보 담아줌
pid_t wait4 (pid_t, pid, int *statloc, int options, struct rusage *resage); // wait3 에서 pid 지정 가능
 return waitpid와 동일하게 기능;

/* 14. execl, execv, execle, execve, execlp, execvp : 새로운 프로그램 대체 
 *   l : char* arg0, arg1, arg2, ... , (char*)NULL : 리스트 타입으로 argument 전달 (arg0 : 파일명, ★마지막은 항상 NULL);
 *   v : argv[] 으로 argumnet 전달
 *   p : 경로 지정없는 실행파일 실행    (PATH 에서 실행파일을 찾음)
 *   e : 환경변수 envp[] 전달 // NULL일 경우 전역변수 environ 사용
 *
 *    exec 계열 함수들을 기존의 부모프로세스를 자식프로세스가 '완전히 대체'해서 사용
 *         : 현재 프로세스의 이미지 (텍스트, 자료, 힙, 스택 구역) 들은 기존파일에서 완전히 대체
 *         ++ 실행파일의 경우 기계어 코드이면 직접 실행 OR 아니면 '스크립트 쉘' 로 인식하여 "/bin/sh" 실행
 *         ++ 명령라인 arg들과 env[] 최대 개수는 ARG_MAX 상수에 의해 제한될 수 있음
 *         ++ 새로 실행된 프로세스의 파일디스크립터(fd) 는 부모프로세스가 오픈하면 그대로 오픈되어 있지만,
 *            부모 프로세스 시그널 값은 초기 설정으로 복원된다. -> 호출 프로세스가 설정한 handler 역시 초기설정으로 복원된다! (핸들러가 없는 상태로 작동)
 *            부모 프로세스에서 ignore 되었던 시그널은 계속 ignore 상태가 된다!
 *         ++ 하지만, 경보(alarm) 까지 남은 시간 등은 상속된다.
 */
