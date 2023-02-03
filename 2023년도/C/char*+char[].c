/*
string 표현 시
char *name 과 char name[] 형태로 표현할 수 있다.

<ex>
char *name = "JUNHYEONGaa";
char name2[] = "JUNHYEONGaa";

name2[] 로 설정하면 자동으로 배열값 크기를 계산해주어 sizeof(name2) 를 하면 크기가 나온다. 
    (\0 포함해서 1글자 더 늘어난다.) 하지만, name[3] = "h" 과 같이 값을 변경하진 못한다.
    
*name 으로 설정하면 값 변경은 가능하지만, sizeof(name) 값이 8 (주솟값 크기) 로 고정된다는 단점이있다.

=> 두개 보완해서 string 을 써도된다. <string.h>
*/


#include <stdio.h>
int main(void)
{
    char *name = "JUNHYEONGaa";
    char name2[] = "JUNHYEONGaa";
    
    
    printf("%ld\n", sizeof(name)); //8
    printf("%ld\n", sizeof(name2)); //12
    printf("%c\n", name[10]);
    return 0;
}
