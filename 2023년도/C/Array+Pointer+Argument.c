/*  
    1. 포인터 함수 (함수 명 변경)
    (type)(*name)(Argument,...) = function 으로 사사용
        ex>
        void print(int* a, int b);
        void(*바꿀 변수명)(int*,int) = print;
        로 사용.
        
        
    2. 다차원 배열의 포인터와 매개변수 전달방식.
    - 2차원 배열의 경우.
        int arr[3][4] 를 포인터형으로 전달하려면
        int (*ptr_arr)[4] = arr; 꼴이된다. //여기서 괄호가 아주 중요하다 !!!
        
        이것을, 매개변수로 전달하려면 다음과 같이 전달하면 된다.
        void print_arr(int (*arr)[4]);   //괄호가 아주 중요하다 !! x 2번강조
        void print_arr(int [][4]); //형태로 전달해도 된다.
         여기서 int*[5] 의 사이즈는 40이다. (주솟값크기 8 x 5 = 40)
         sizeof(int*[5]) == 40
        
    - 3차원 배열 이상 다차원 배열의 경우
        int arr[3][4][5] 의 포인터형은 int (*ptr_arr)[4][5] = arr;이다.
        매개변수로 전달할 경우, 꼴 형태가 복잡해지므로 다음과 같은 방식을 도입한다.
            -> void* 형태로 전달하고 내부에서 수정..
            
        void print_arr_3 (void *arr)
        {
            int (*ptr_arr)[4][5] = arr;
        }
        
        void print_arr_3 (void arr[][4][5]) 도 사용가능.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct man{
    char *name;
    int age;
    int money;
}Man;



void push_value(Man* m)
{
    m->name = "Agent1";
    m->age = 22;
    m->money = 1000000;
}

void print_element(Man *m)
{
    printf("%s age is %d and money is %d\n", m->name, m->age, m->money);
}


void setting(int (*arr)[5])
{
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 5 ; j++)
            arr[i][j] = (i+1)*(j+1);


}

void print_array(int (*arr)[5])
{
   for (int i = 0 ; i < 4 ; i++)
   {
        for (int j = 0 ; j < 5 ; j++)
            printf("%d ",arr[i][j]);
        printf("\n");
   }
    printf("\n");
}


void setting_3 (void *arr);
void print_array_3(void *arr);
int main (void)
{
    Man me;
    /* 함수 명 변경 */
    void(*push)(Man*) = push_value;
    void(*print)(Man*) = print_element;
    
    /* 바뀐 함수 명 사용.*/
    push(&me); //구조체 자체는 포인터가 아닌 일반변수므로 &사용.
    print(&me);

    Man *you = (Man*)malloc(sizeof(Man)); //구조체를 동적할당하면 변수명이 포인터므로 매개변수 전달 가능.
    push(you);
    print(you);
    
    
    
    /* 다차원 배열과 함수인자*/
    int arr[4][5] = {0,};
    setting(arr);
    void(*print_a)(int *[5]) = print_array;
    print_a(arr);
    
    /* 2차원 배열의 포인터형은 int (*변수명)[열길이]  로 설정한다. 괄호가 중요하다.*/
    int (*ptr_arr)[5] = arr;
    print_a(ptr_arr);
    printf("%ld\n", sizeof(int*[5])); //40 (8(주솟값크기) * 5 = 40)
    
    
    /* 3차원 배열을 매개변수로 전달하는 경우*/
    int arr2[3][4][5];
    setting_3(arr2);
    print_array_3(arr2);
    
    
    return 0;
}

void setting_3 (void* arr)
{
    /*2차,3차 공간 길이를 아는 경우.*/
    int (*ptr_arr)[4][5] = arr;
    for (int i = 0 ; i < 3 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            for (int k = 0 ; k < 5 ; k++)
                ptr_arr[i][j][k] = (i+1)*(j+1)*(k+1);
}


void print_array_3 (void *arr)
{
    int(*ptr_arr)[4][5] = arr;
    for (int i = 0 ; i < 3 ; i++)
    {
        for (int j = 0 ; j < 4 ; j++)
        {
            for (int k = 0 ; k < 5 ; k++)
                printf("%d ",ptr_arr[i][j][k]);
            printf("\n");
        }
        printf("\n");
    }

}
