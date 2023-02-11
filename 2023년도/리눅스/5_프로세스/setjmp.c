#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf glob_buffer;
void ssu_nested_func (int loc_val, int loc_volatile, int loc_register);

int main(void)
{
    register int loc_register;  /*레지스터 변수*/
    volatile int loc_volatile;  /*최적화를 해도 이전의 값으로 되돌리지마라!*/
    int loc_val;

    loc_register = 12;
    loc_volatile = 11;
    loc_val = 10;

    /** 자기 호출이 아니라면(== longjmp라면) if 진행*/
    if (setjmp(glob_buffer) != 0)
    {
        /** setjmp 위치 레지스터값/스택 값 저장*/
        printf("After longjump[loc_val=%d, loc_volatile=%d, loc_register=%d]\n",
        loc_val,loc_volatile,loc_register);
        exit(0);
    }

    /** */
    loc_register = 82;
    loc_volatile = 81;
    loc_val = 80;
    ssu_nested_func(loc_val, loc_volatile, loc_register);
    exit(0);
}

void ssu_nested_func (int loc_val, int loc_volatile, int loc_register)
{
    printf("Before longjump[loc_val=%d, loc_volatile=%d, loc_register=%d]\n",
    loc_val,loc_volatile,loc_register);
    longjmp(glob_buffer, 1);
}

/**
 * 컴파일 최적화 하고싶으면 tasks.json 에서 마지막에 -O 추가할것.
 *  컴파일 최적화 시 loc_register, loc_val 값은 이전값(12,10) 으로 되돌아감
 *  하지만 volatile 변수는 최적화하더라도 되돌아가지않음.
 * 
 * <질문 : 최적화 사용 전인데 loc_val 값이 10으로 되지 않습니다..>
 * Before longjump[loc_val=80, loc_volatile=81, loc_register=82]
 * After longjump[loc_val=80, loc_volatile=81, loc_register=12]
 * 
 * < 최적화 옵션 -O 추가시 (대문자 O))
 * Before longjump[loc_val=80, loc_volatile=81, loc_register=82]
 * After longjump[loc_val=10, loc_volatile=81, loc_register=12]
*/
