/* 0. Shell 문법 */
 1. ./실행파일 &            : 해당 실행파일 pid를 알 수 있음;

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
