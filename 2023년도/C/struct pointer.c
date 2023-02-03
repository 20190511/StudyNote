/*
    1. 가장 중요한 개념 ㅣ 구조체 변수는 포인터가 아니다! |
    -> 구조체 변수는 구조체안의 타입만큼 크기를 가진 새로운 타입으로 인식하지 포인터가 아니다!
        그래서 매개변수로 포인터를 넘겨줘야한다면, &구조체변수 로 넘겨야한다.
        
        ex>
        void make_man (Man *man1);
        ...
        Man m1;
        make_man (&m1);
        
        Man *m1 = (Man*)malloc(sizeof(Man));
        make_man(m1);
        
        이런식이다.
        ★ 구조체 변수의 주솟값은 멤버 변수 중 첫 번째 멤버의 주솟값이다.
            아래 예제에선 &name 이 구조체의 주솟값이다.
    
    -> 구조체는 char name[20]; 를 초기화할 때 초기화 블럭을 사용한다면
        strcpy()를 사용하지 않고 사용가능하다.
    
    
    2. 복습타임 : -> 와 . 의 차이
        *a.age == a->age 이다. 즉 *변수.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct man {
    char *name;
    int age;
}Man;


void make_man (Man *man1)
{
    // strcpy(man1->name, "JH"); /* char name[20]; 일때 사용*/
    man1->name = "JH";
    man1->age = 22;
}


int main(void)
{
    // person 자체는 포인터지만, person[0] 은 Man type 이다.
    Man** person = (Man**)malloc(sizeof(Man*)*20);
    printf("sizeof(Man*) = %ld\n", sizeof(Man*));
    
    person[0] = (Man*)malloc(sizeof(Man));
    
    make_man(person[0]);
    printf("%p\n", person[0]);
    printf("%s %d\n",person[0]->name, person[0]->age);
    
    /* struct Man 의 주솟값은 첫 번째 멤버의 주솟값과 동일하다.*/
    printf("%p, %p", &person[0]->name, person[0]);
    return 0;
}
