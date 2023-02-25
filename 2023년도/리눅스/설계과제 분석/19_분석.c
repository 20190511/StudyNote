/** 파라미터로 넘겨진 dirp 디렉토리에 있는 파일 및 디렉토리 목록을 filter함수에서 정제하여 compar의 비교 조건으로 sorting합니다. 
 * 이 함수는 opendir(3), readdir(3), closedir(3)을 한번에 처리하고 filter와 sorting 기능을 갖는 함수입니다. 
 * 이 함수를 통하여 파일명/디렉토리명 순서나 생성 시간 순서 등으로 sorting하고 정해진 규칙(filter)의 파일/디렉토리만 목록으로 얻을 수 있습니다.
 <인자 설명>
  dirp
      - 파일 목록 또는 디렉토리 목록을 얻을 디렉토리에 대한 절대 path 또는 상대 path
  
  namelist : dirent 구조체 배열이 저장됨 (struct dirent **namelist = NULL 으로 초기화하고 사용할 것) -> dirent 포인터 배열 (**) + Call By Pointer (*)
      - namelist에 디렉토리에 있는 파일 및 디렉토리 목록이 저장됩니다.
      - 내부적으로 malloc(3) 또는 realloc(3)으로 할당되므로 사용후에는 반드시 free(3)를 해야합니다. 
      - 다중 pointer 변수를 선언해서 사용하므로 건별 free(3) 후 namelist 자체도 free(3)를 해야합니다. (예제 참조)
  filter
      - namelist에 포함시킬 것인지 여부를 판단하는 함수에 대한 pointer.
      - 이 filter함수의 return 값이 0이면 namelist에 포함시키지 않고 0이 아니면 포함시킵니다.
      - NULL이면 filter없이 파일 및 디렉토리 전체가 namelist에 저장됩니다.
      - . 및 .. 디렉토리도 포함되어 있음.
  compar
      - 데이터를 sort할 비교함수에 대한 포인터입니다.
      - 내부적으로 qsort(3)를 사용하므로 이 함수를 통하여 sorting합니다. 
      - 만약 이름(struct dirent 구조체의) d_name으로 sorting하려고 한다면 이미 구현된 alphasort(3)함수를 사용할 수 있습니다.
      - 만약 이 compar를 NULL로 설정하면 sorting없이 출력됩니다.
      
      
 <return>
  -1
      - 오류가 발생하였으며, 상세한 오류 내용은 errno에 설정됩니다.
      ENOENT : dirp 디렉토리가 존재하지 않는 path입니다.
      ENOMEM : 메모리 부족으로 처리되지 않았습니다.
      ENOTDIR : dirp가 존재는 하나 directory가 아닙니다.
  0 이상
      - 정상적으로 처리 되었으며, namelist에 저장된 struct dirent *의 갯수가 return됩니다.
       즉 디렉토리 내부에 있는 파일/디렉토리 개수가 리턴
*/
#include <dirent.h>
int scandir(const char *dirp, struct dirent ***namelist,
      int (*filter)(const struct dirent *),
      int (*compar)(const struct dirent **, const struct dirent **));







/** 2. strrchr : 뒤에서부터 ch가 있는 위치를 알려주는 함수 
                 문자열에 특정 문자가 있는 위치를 뒤에서부터 찾는 함수 (포인터)*/
#include <string.h> 
const char* strrchr(const char* str, int character);
char* strrchr(char* str, int character);


/** 이중 포인터 해제 **/
struct dirent** nameliset;
// ... ~~~동적할당~~~ ...
for(i=0; i<count; i++){
        free(namelist[i]);
}                                                      
free(namelist);  // 이중포인터 해제 시 각 포인터를 해제해준 다음 자기자신 해제!





/** 3. 스레드 중간취소 및 킬 ***/
static void killer(void *arg) {
    
    pthread_t *tids = (pthread_t*)arg;
    //학생파일실행(t_function) 스레드에 최소 요청을 보냄
    pthread_cancel(tids[0]);
}

// 재해석 요지 필요
static void *control(void *arg) {
    
    pthread_cleanup_push(killer, arg);
    do {
        pthread_testcancel();
    } while(1);
    pthread_cleanup_pop(1);
    return (void*)(NULL);
}
