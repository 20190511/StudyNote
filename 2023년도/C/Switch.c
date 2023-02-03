/*
1. switch 문 : case 변수=1 이여도 break가 없으면 아래를 계속 실행해줌. (중요!) 
switch(변수)
{
    case 1:
        ~~;
    case 2:
        ~~;
        break;
}


2. (*num)++ 와 *num++ 의 차이
    (*num)++ 는 포인터안의 값을 변경시키는 것이고
    *num++ 는 포인터 주솟값을 4만큼 증가시키는 것이다.
    
    
3. 매개변수에 복사되어 전달되는것이 싫다면
    포인터값으로 받고 주솟값을 넘겨주면 복사연산이 줄긴한다.
    
*/
#include <stdio.h>

int switch_example(int *num)
{
    int check = 0;
    switch(*num)
    {
        case 1:
            printf("case 1\n");
            (*num)++; //*num++ 해버리면 주솟값이 변경되버림.
            check++;
        case 2:
            printf("case 2\n");
            (*num)++;
            check++;
        case 3:
            printf("case 3\n");
            (*num)++;
            check++;
        case 4:
            printf("case 4\n");
            check++;
    }
    
    return check;
}



int main (void)
{
    int a = 4;
    int check = switch_example(&a);
    printf("%d\n",check);
    printf("%d\n", a);
    
    //즉 switch문은 break를 넣어주지 않으면, 쭉 내려가면서 break가 있는 문장까지
    실행된다.
    a = 1;
    check = switch_example(&a);
    printf("%d\n",check);
    printf("%d\n", a);
    
}
