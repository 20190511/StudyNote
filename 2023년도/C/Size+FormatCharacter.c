/*
★ C언어에서는 bool 자료형이 있다. 이를 활용하려면, <stdbool.h> 를 받아오면된다.
아니면, typedef enum {true, false} bool; 과 같이 사용해도 좋다.


★ long과 int 의 크기는 컴파일러마다 달라지므로, 
sizeof(int) 와 같은 형태로 사용하는 것이 안전하다.

<형식문자 정리>
%c - 하나의 문자, ASCII 문자로 출력 (char, short, int) //short,int는 자동으로 char로변환.
%s - 문자열 (char*)
%p - 포인터의 주소 값 (void*)


%d - 부호가 있는 10진수 정수 (char, short, int)
%ld - 부호가 있는 10진수 정수 (long)
%lld - 부호가 있는 10진수 정수 (long long)

 
%u - 부호가 없는 10진수 정수 (unsigned int)
%o 부호가 없는 8진수 정수 (unsigned int)
%x, %X - 부호가 없는 16진수 정수 (unsigned int)

%f - 10진수 방식, 부동소수점 실수 (float, double)
%Lf - 10진수 방식, 부동소수점 실수 (long double)

%e, %E - e, E 방식, 부동소수점 실수 (float, double)
%g, %G - 값에 따라 자동으로 %f 혹은 %e 중에서 선택 (float, double)
*/



#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    printf("%ld\n", sizeof(char)); //1
    printf("%ld\n", sizeof(short)); //2
    printf("%ld\n", sizeof(int)); //4
    printf("%ld\n", sizeof(float)); //4 <- float도 4바이트다...
    printf("%ld\n", sizeof(long)); //8
    printf("%ld\n", sizeof(long long)); //8

    printf("%ld\n", sizeof(double)); //8
    printf("%ld\n", sizeof(long double)); //16
    
    
    
    
    
    /* 포인터 값의 size는 모두 8이다 잘 기억하라! (주솟값의 크기니까 당연하다.)*/
    printf("%ld\n", sizeof(char*)); //8
    printf("%ld\n", sizeof(int*)); //8
    printf("%ld\n", sizeof(long long*)); //8
    printf("%ld\n", sizeof(bool*)); //8
    
    return 0;
}
