/*
    1. typedef 는 추가적으로 이름을 붙이고 싶을 때 사용한다. ex) int 를 INT 로 부르고 싶을 때,
    typedef ... ... ... 변환할 이름.



    2. enum은 (변수명)의 타입으로 아래 상수들을 정의하겠다. 
        (상수들만 늘여놓으면 0부터 1,2, ... 형태로 배치됨)
    
    enum 변수명 { 
        상수1, 상수2, 상수3
    }
    
    예제 enum>
    enum don {
        won = 1000, dall=1, en=10
    };
    enum don k;
    for (k=dall ; k<won ; k++)
        printf("s\n");
    //즉, 사용할 때, s가 999번 출력될 것이다.
    
    
    예제2 : typedef + enum > 
    typedef enum don{
        won = 1000, dall=1, en=10
    };
    don k;
    for (k=dall ; k<won ; k++)
        printf("s\n");

*/


#include <stdio.h>

typedef int INT;
typedef long long ll;


//Car 변수명            호출가능
//struct Car 변수명     호출가능
struct car {
  char *name;
  int speed;
};
typedef struct car Car;


//P 변수명              호출가능
//struct P 변수명       호출불가능
typedef struct{
   int k; 
}P;


typedef enum don{
    won=4, dall=1, en=2
}Don;
    
int main(void)
{
    INT a = 3; 
    ll k = 9;
    printf("%d\n", a); //3
    printf("%lld\n", k);  //9
    
    Car c = {"Vector", 30};
    printf("%s, %d\n\n", c.name, c.speed);  //vector, 30
    
    
    Don en;
    for (en=dall ; en<won ; en++)
        printf("s\n");
    return 0;
}
