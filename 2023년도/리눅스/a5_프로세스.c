/* 0. Shell 명령어 및 Others */
 1. find            : 현재 할당된 메모리 값들을 볼 수 있음;


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
 *  자원 한계에는 약한한계(Soft Limit) 와 강한한계 (Hard Limit)
 */
#include <sys/resoucre.h>
#include <sys/times.h>
int getrlimit (int resource, struct rlimit *rlptr);
int setrlimit (int resource, struct const rlimit *rlptr);
