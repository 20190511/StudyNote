/*
const 개념
int* const b = &a; // b가 가리키는 대상은 변경이 불가능하다. + *b 의 값 변경은 가능하다.
const int *b = &a; // b가 누구를 가리키는지는 상관없다 + *b 의 값 변경은 불가능하다.
    // 포인터b 를 묶었냐 or b를 묶었냐 그 차이다.
    
int a = 3;
int c = 5;
    
int* const b = &a;
b = &c;   // 컴파일 에러
*b += 3;  // 성공


const int *b = &a;
b = &c;   // 성공
*b += 3;  // 컴파일 에러. <- a같은 간접적인 변수로 변경해야함.



static 개념 : 변수 or 함수를 생성하는 블럭 범위 내에서만 전역변수화 처리해서 쓰겠다!
    -> 즉, 해당 변수(함수, ..) 등을 해당 "블럭 내에서만" 쓰겠다.

: 함수 or 구조체 or 블럭 내에서 static을 사용하는 경우.
    ★ 하지만 외부에서 접근은 안된다.!
    -> 외부 접근 전역변수를 쓰러면 진짜 전역변수를 써라!
    
    static int a; //(static int a = 0; 과 동일함)
    
    
    
extern 개념 : 변수 or 함수를 다른 파일 등 외부에서도 사용하겠다. 
    # 하지만, 함수에 대해서는 extern을 없애고 선언만 해줘도 같은 의미이긴하다.
    -> ★ 선언은 어느 파일에서도 할 수 있다. (stdio.h 파일의 변수를 여기서 extern 해도된다.)
    가볍게 호출하는 용도로 쓰자. (파일 분할 공부할 때 다시 보기로하자.)

extern int num;   //int 형 변수 num은 include 된 모든 파일에서 사용 가능하다.
extern void foo(); //void 형 함수 foo()를 전 범위에서 쓰겠다!

*/
#include <stdio.h>
int fo = 5;
void foo()
{
    // 전역변수화 처리되었음. foo=0이라고 적혀있어도 실행되지않는 코드 
    static int fo = 5; 
    fo++;
    printf("%d\n",fo);
}


int main(void)
{
    int a = 3;
    int c = 5;
    int const *b = &a;
    a += 4;
    printf("%d\n",*b);
    
    
    for (int i = 0 ; i < 5 ; i++)
    {
        foo();
        //printf("%d\n", fo);
    }
    return 0;
}
