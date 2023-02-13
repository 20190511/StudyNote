/* 0. Shell 문법 */
 1. ./실행파일 &            : 해당 실행파일 pid를 알 수 있음;
 2. kill -10 <PID>         : 해당 PID 프로세스에 SIGUSR1 시그널 전달.

/* 0.1. C 언어 테크닉 */


/* 1. signal : 시그널을 제작하는 함수
 *           해당 값 handler값이 뒤와같으면, SIG_IGN : 무시, SIG_DFL : 기본값 설정 이다.
 *           LINUX 커널 2.4에서는 시그널이 발생되면 BLOCKED 비트로 마스크 처리하여 해당 시그널이 중첩되지 못하게하였으나
 *           LINUX 커널 2.6에서는 시그널이 발생될 때 count++를 해주는 방식으로 바뀌는 추세이다.
 *           ++ (Linux 2.6) 자식<-> 부모 프로세스 사이의 공유되는 시그널함수들은 task_strcut 에 저장되어 공유되고
 *                          공유되지 않는 시그널함수들은 pending->sigpending 에서 따로 중첩되어 기록된다.
 *           시그널이 발생되면 우선 pending (명령미처리) 상태에 들어가있게되고 적절한 시점에 pending 에서 시그널을 처리하게된다.
 *           시그널 처리함수 handler는 매개변수 타입이 int형인데 이는 '처리할 signal number' 이다
 */
#include <singal.h>
sighandler_t signal (int signum, sighandler_t handler);     // signal 제작함수 (해당  signo 를 handler로 처리하겠다)
  return 성공시 이전 핸들러 함수 포인터(주솟값), 실패시 NULL;
typedef void (*handler) (int);                              // 처리할 핸들러함수 (void타입, int매개변수) '


/* 2. kill, raise : 시그널 보내기
 *           kill (pid, sig) : 해당 pid에 sig 시그널 보내기 (시그널 전달)
 *              pid < 0      |  '보낼 권한이 있으면서 프로세스 그룹ID의 pid에 해당하는 모든 프로세스에게 시그널' 전달.
 *              pid == 0     |  자신과 '같은 그룹에 속한 모든 프로세스에게 시그널 전달' (단, 시스템 프로세스 제외)
 *              pid == -1    |  시그널을 '보낼 권한이 존재하는 그룹프로세스ID가 pid인 모든 프로세스' 에게 시그널 전달
 *              pid > 0      |  해당 프로세스에게 시그널 전달.
 *
 *           raise (sig)     : 자기 자신에게 sig 시그널 보내기 (시그널 전달)
 *           수신자 권한 (RUID(실제), EUID(유효)) == 송신자 권한 (RUID(실제), SUID(저장된)) 이어야 시그널 호출 가능
 */
#include <sys/types.h>
#include <signal.h>
int kill (pid_t pid, int sig);                             // 해당 pid에 sig 시그널 전달(전송)
  return 성공시 0, 실패시 -1 ->errno 설정;

#include <signal.h>
int raise (int sig);                                       // 자기자신에게 sig 시그널 전달 (전송) 
  return 성공시 0, 실패시 0이 아닌 값;

/* 3. alarm, pause : 프로세스에 SIGALRM (경보) 호출 
 *           alarm 디폴트 액션은 '프로세스 종료' 이다. <- 캐치하지 않거나 무시한 경우 프로세스 종료
 *                 만약 alarm이 존재하는 상태에서 재호출 하였을 시 : 남은 알람시간 return + 새로운 알람으로 교체
 *                 ++활용 방법으로는 핸들러함수에 alarm(??) 를 넣어서 SIGALRM 을 무한호출시킬 수 있다.
 *                 alarm(0) 을 넣으면 알람 취소가 된다
 &
 *           pause 디폴트 액션은 '프로세스 종료' or '시그널 핸들러 호출시그널이 발생할 때까지 기다린다'
 *                 즉, 시그널이 발생할 때까지 프로세스를 멈춰놓겠다라는 함수
 */
#include <unistd.h>
unsigned int alarm (unsigned int seconds);                  // 해당 seconds 뒤에 SIGALRM 시그널 호출 
  return 이전에 설정된 타이머가 없으면 0, 있으면 이전의 알람의 남은 시간;
int pause (void);                                           // 다음 시그널이 올 때 까지 

/* 4. sigemptypse, sigfillset, sigaddset, sigdelset, sigismember : 시그널 집합(Signal Set) 비트처리함수
 *           시그널 마다 1개씩 비트를 처리하여 LINUX에는 63개의 시그널에 대해 63비트의 int 배열이 있다
 *           후에 이 비트값이 1이면 해당 함수들에 영향을 받음을 의미함 (ex)bitmask)
 */
#include <signal.h>
int sigemptyset (sigset_t *set);                            // set의 모든 멤버 비트값을 모두 0으로 만들겠다
int sigfillset (sigset_t *set);                             // set의 모든 멤버 비트값을 모두 1으로 만들겠다
int sigaddset (sigset_t *set, int signo);                   // set에 signo 멤버를 추가하겠다
int sigdelset (sigset_t *set, int signo);                   // set에 signo 멤버를 삭제하겠다.
 return 성공시 0, 실패시 -1 -> errno 설정
int sigismemeber (sigset_t *set, int signo);                // 해당 signo 가 set 에 포함되어있는가(1인가?)
 return set에 signo가 멤버이면 1 아니면 0, 실패시 -1 -> errno 설정;


/* 5. sigprocmask : 해당 시그널 집합을 how에 따라 처리하는 함수
 *           sigprocmask(how,set,oldset) 중 how는 아래 3가지 모드로 set과 현재 마스크집합을 비교하여 처리하는 연산을 해준다.
 *           <how 상수>
 *           SIG_BLOCK    : 현재 시그널집합 ∪ set | 블락 시그널을 추가할 때 사용
 *           SIG_UNBLOCK  : 기존의 블락된 시그널에서 set의 시그널을 제거함
 *           SIG_SETMASK  : 해당 set을 새로운 시그널집합으로 사용하겠다.
 *           
 *           *oldset 의 경우에는 바뀌기 전 현재 시그널 집합 구조체가 담기게된다. (NULL 인 경우 저장되지 않음)
 *           ★ set=NULL 이면 how 에 무슨값이 들어가도 새로운 마스크집합이 생성되지 않는다.
 */
#include <signal.h>
int sigprocmask (int how, sigset_t* set, sigset_t* oldset);  // how 에 따라 set을 설정, oldset에는 바뀌기 전 마스크집합 설정
 return 성공시 0, 실패시 -1 -> errno 설정;

/* 6. sigpending : 블락된 시그널 + 현재 Pending(미처리된) 시그널인 상태의 시그널 집합을 가져오는 함수 
 *           BLock+Pending 시그널을 sigset_t 구조체로 시그널집합을 가져온다.
 *           위에 설명된 대로 가져온 시그널 집합을 sigset_t *set 으로 저장한다
 */
#include <signal.h>
int sigpending (signal_t *set);                             // set에 지금 Block됨 + ★ Pending 된 상태의 시그널 집합을 저장
 return 성공시 0, 실패시 -1 -> errno 설정;


/* 7.sigaction : signal() 을 구조체로 전달하여 현대에 더 많이 쓰는 시그널 수정함수 
 *           해당 signo에 대하여 구조체 sigaction 상태로 만들어준다. (+ 그리고 바뀌기 전 상태는 oldact 구조체에 저장)
 *           시그널을 구조체 sigaction 를 이용하여 전달해준다. sigaction 구조체는 다음의 멤버변수를 가진다.
 *           <sigaction>
 *           void (*sa_handler) (int);                    : 해당 시그널에서 호출할 함수
 *           sigset_t sa_mask;                            : 해당 시그널 실행 전 블락을 할 시그널set
 *           int sa_flags;                                : 시그널 옵션 (
 *           void(*sa_sigaction)(int, siginfo_t *, void*) : sa_flags 가 SA_SIGINFO 일 때 인자를 2개 더 전달해주는 시그널 함수
 *           ++ 중요한 점은 sa_sigaction을 실행할 때는 위의 sa_handler와 같이 사용하면 안됨 (동일 메모리공간을 사용하는 경우있음)
 *           +++ 해당 sigaction() 을 호출하면 해당 상태값이 다음 sigaction() 이 호출되기 전까지 상태를 유지한다. (sa_masks가 유지됨)
 *           +++ 그러므로 signal() 호출 후 sigaction()을 호출했을 때, oldact는 보장해주지않는다.
 */
#include <signal.h> //해당 signo 를 act 구조체 형태로 시그널 수정 + oldact에는 바뀌기전 상태의 시그널 sigaction 구조체가 들어감.
int sigaction (int signo, const struct sigaction *act, struct sigaction *oldact); 
 return 성공시 0, 실패시 -1 -> errno 설정;

/* 8. sigsetjmp, siglongjmp : setjmp, longjmp에서 현재 시그널 비트 셋을 그대로 들고갈 것인지 물어보는 savesgis를 추가한 분기함수
 *           기존 longjmp,setjmp 는 시스템 핸들러 처리 중 분기를 한다면 자동으로 해당 시그널에서 자동으로 '블락' 처리된다
 *           그 상태에서 return 되지않고 longjmp를 해버린다면 나중에 시스템호출 | 함수 호출에서 시그널 마스크의 복원이 안 될 수 있다
 *           이를 해결하기위해 'savesigs' 변수를 두어 해당 변수가 0이아니라면 현재 시그널 마스크를 env에 저장하여 호출하게해준다.
 */
#include <setjmp.h>
int sigsetjmp (sigjmp_buf env, int savesigs);              // savesigs!=0 이면 현재 signal set를 env에 저장하여 분기
 return 직접호출하는 경우에는 0, 호출해서 리턴된 경우에는 0이 아닌값;
void siglongjmp (sigjmp_buf env, int val);                 // sigsetjmp 부분으로 분기.

/* 9. sigsuspend : 해당 sigmask 를 시그널 마스크 집합으로 설정하고 + Pause() 처럼 다른 시그널을 기다렸다가 + 원래의 시그널 마스크 집합으로 설정
 *           sigprocmask() 와 pause() 를 원자적으로 실행하는 함수이다 (마스크셋을 설정하고 다른시그널을 기다리는 동안 가로챌 수 없고,
 *                                                                   이전의 시그널 마스크 비트로 되돌려준다)
 *           원자적이란 : 연산 도중 가로챌 수 없으며, 값이 바뀌지 않는 것을 의미한다.
 *           물론, SIGKILL 이나 SIGSTOP의 경우에는 마스크에 포함시켜도 블락되지않는다. (그리고 프로그램종료)
 */
#include <signal.h>
int sigsuspend (const sigset_t *sigmask);                 // 해당 시그널 마스크로 다른 시그널이 올 때 까지 기다렸다가 원상태 시그널 마스크 set 복귀
 return 항상-1 ★, errno는 EINTR 로 설정;


/* 10. abort : 프로세스 자신에게 SIGABRT 호출하여 강제종료  
 *           프로세스들은 SIGABRT를 무시할 수 없다. 대신 별도의 시느러 핸들러를 두고 종료하기 전에 여러가지 정리작업을 할 수는 있다.
 */
#include <stdlib.h>
void abort(void);                                        // 자신에게 SIGABRT 시그널을 호출시키며 프로세스 강제종료

/* 11. sleep : 프로세스를 seconds 만큼 일시중지시키는 함수
 *           프로세스 스케쥴과 같은 시스템 다른활동으로 정확하게 seconds 만큼 일시정지 하지는 않을 수 있다
 *           alarm() 과 비슷하나 , alarm(10) 을 호출하고 3초 뒤에 sleep(5); 를 호출한다면 sleep은 5초후에 return 하지만
 *           alarm도 원래대로 2초뒤에 return 하는지의 여부는 시스템마다 종속적으로 발생한다.
 */
#include <unistd.h>
unsigned int sleep (unsigned int seconds);               // Seconds 만큼 프로세스 일시중지
 return 성공시 0, 중간에 다른 sleep가 올 시 남은 초 수;


/* 11. sleep : 
