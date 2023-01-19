#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// h(x) = x mod 13 선형조사
// h(x) = (x+i**2) mod 13 2차원 조사

//선형 조사법
int hash_function1(int address, int count)
{
    return (address + count) % 13;
}
//이처원 조사법
int hash_function2(int address, int count)
{
    return (address + (int)pow(count, 2)) % 13;
}
//개방 주소법으로 인덱스 찾기
int find_OpenAddressing(int method ,int array [],int key)
{
    int count = 0;
    int index = 0;
    while (1)
    {
        //method = 1 : 선형주소법 , method = 2 : 이차원 조사법
        if (method == 1)
            index = hash_function1(key, count);
        else if (method == 2)
            index = hash_function2(key, count);

        if (array[index] == key)
            break;
        //해당하는 key값을 찾지 못한 경우.
        else if (count > 12)
        {
            index = -1;
            printf("찾는 값이 없습니다. -1을 반환합니다.");
        }
        count++;
    }
    return index;
}


//출력함수_마지막 4개의 값에 따른 인덱스를 출력해줌
void print_index(int num_array[] , int find_key [])
{
    for (int i = 0; i < 4; i++)
    {
        printf("key = %d , index = %d \n", find_key[i], num_array[i]);
    }
}
//출력함수 _ 전체 테이블을 형태를 출력해줌
void print_hash(int num_array[])
{
    for (int i = 0; i < 13; i++)
    {
        printf("Oper_Addresss_arr[%d] = %d \n",i, num_array[i]);
    }
}

/* Chaining 방식 함수들 */
//체이닝 방식 
typedef struct _Chain {
    int data;
    struct _Chain* link;
}Chain;

//체인 해시테이블 생성 = 연결리스트 방식으로 해시함수를 설정하기 위함.
Chain* table[13];
void input_chain(Chain* root, int key)
{
    int hash_value = key % 13;
    //table이 비어있으면 헤드노드로설정.
    if (table[hash_value] == NULL)
        table[hash_value] = root;
    else
    {
        root->link = table[hash_value];
        table[hash_value] = root;
    }
}

//link주솟값 반환.
Chain* findData(int address_value)
{
    //1번 문제와 맞게 해당 인덱스 수정
    int hash_value = address_value % 13;
    //헤드 노드가 없는 경우,
    if (table[hash_value] == NULL)
        return NULL;
    //base Addresss인 경우
    if (table[hash_value]->data == address_value)
    {
        printf("this value is BaseAddress in Hash Table\n");
        printf("key = %d , index = %d\n\n", address_value , hash_value);
        return table[hash_value];
    }
    //연결리스트로 연결되어 있는 경우
    else
    {
        Chain* new_node = table[hash_value];
        while (new_node->link)
        {
            if (new_node->link->data == address_value)
            {
                printf("this value is Collision value , so its structure is linkedlist\n");
                printf("key = %d , base index = %d , linked previous data = %d \n\n", address_value, hash_value, new_node->data);
                return new_node->link;
            }
            new_node->link;
        }
    }
    return NULL;
}
//chain실행함수
void execute_chain(int array[], int input_data, int count)
{
    count = 0;
    Chain* node = (Chain*)malloc(sizeof(Chain));
    node->data = input_data;
    node->link = NULL;
    input_chain(node, node->data);
    array[hash_function1(input_data, count)] = node->data;
}

//연결된 전체적인 형태를 출력해주는 함수
void printChainNode()
{
    for (int i = 0; i < 13; i++)
    {
        printf("chain_array [%d] =", i);
        if (table[i] != NULL)
        {
            Chain* print_node = table[i];
            while (print_node->link)
            {
                printf(" %d ", print_node->data);
                print_node = print_node->link;
            }
            printf(" %d", print_node->data);
        }
        else
            printf(" 0 (NULL value)");
        printf("\n");
    }
    printf(" ****** End ******\n");
    printf("\n");
}


//method = 해시 방법 , array = 배열공간,  input_data = 넣을 데이터
void input_hash(int method, int array[], int input_data)
{

    int address = 0;
    int count = 0;
    //Open Addressing : 개방 주소 방법 (method = 1 : 선형주소법 , method 2 : 이차원 조사법)
    if (method == 1 || method == 2)
    {
        while (1)
        {
            if (method == 1)
                address = hash_function1(input_data, count);
            else
                address = hash_function2(input_data, count);
            if (array[address] == 0)
            {
                array[address] = input_data;
                break;
            }
            else
                count++;
        }
    }
    //Chaining : 체이닝 방식
    else
    {
        execute_chain(array, input_data, count);
    }
}

//03번 문제에 해당함. : 개방 주소법의 마지막 4개의 key 값에 대한 index를 출력해줌
void question1(int num1_array[], int input_data[], int find_key [])
{
    printf(" ****** Question1 Find : Linear Probing ******\n");
    int index_bag1[4] = {0,};
    //선형 조사법으로 만든 해시 테이블 생성
    for (int i = 0; i < 8; i++)
    {
        input_hash(1, num1_array, input_data[i]);
        if (i >= 4)
        {
            printf("Linear Probing : input %d , and print table\n", input_data[i]);
            print_hash(num1_array);
            printf("\n");
        }
    }
    //선형 조사법으로 해당하는 key값을 찾기
    for (int k = 0; k < 4; k++)
    {
        index_bag1[k] = find_OpenAddressing(1, num1_array, find_key[k]);
    }
    print_index(index_bag1,find_key);
    printf(" ****** End ******\n");
    printf("\n");
}

//04번 문제에 해당함. : 이차원 주소법의 마지막 4개의 key 값에 대한 index를 출력해줌
void question2(int num2_array[] , int input_data[] , int find_key [])
{
    printf(" ****** Question2 Find : Quadratic Probing ******\n");
    int index_bag2[4] = { 0, };
    //이차원 조사법으로 만든 해시테이블 생성
    for (int i = 0; i < 8; i++)
    {
        input_hash(2, num2_array, input_data[i]);
        if (i >= 4)
        {
            printf("Quadratic Probing : input %d , and print table\n", input_data[i]);
            print_hash(num2_array);
            printf("\n");
        }
    }
    //이차원 조사법으로 해당하는 key값을 찾기
    for (int k = 0; k < 4; k++)
    {
        index_bag2[k] = find_OpenAddressing(2, num2_array, find_key[k]);
    }
    print_index(index_bag2,find_key);
    printf(" ****** End ******\n");
    printf("\n");
}

//05번 문제에 해당함. : 체이닝 주소법의 마지막 4개의 key 값에 대한 index를 출력해줌
void question3(int num3_array[] , int input_data[], int find_key [])
{
    printf(" ****** Question3 Find : Chaining ******\n");
    int index_bag3[4] = {0, };
    //체이닝 조사법으로 만든 해시테이블 생성
    for (int i = 0; i < 8; i++)
    {
        input_hash(3, num3_array, input_data[i]);
        if (i >= 4)
        {
            printf("Chaining : input %d , and print table\n", input_data[i]);
            printChainNode();
            printf("\n");
        }
    }
    //체이닝 조사법으로 해당하는 key값을 찾기
    for (int k = 0; k < 4; k++)
    {
        //return 주소 값은 존제하나, 연결할 이유가 없기에 
        findData(find_key[k]);
    }
    printf(" ****** End ******\n");
    printf("\n");
}




//해시함수가 제대로 작성되었는지 출력하는 함수들.
// 1. 선형 조사법으로 만든 해시테이블
void question1_print(int num1_array[])
{
    printf(" ****** Print : Linear Probing ******\n");
    print_hash(num1_array);
    printf(" ****** End ******\n");
    printf("\n");
}

// 2. 이차원 조사법으로 만든 해시 테이블
void question2_print(int num2_array[])
{
    printf(" ****** Print : Quadratic Probing ******\n");
    print_hash(num2_array);
    printf(" ****** End ******\n");
}

// 3. 체이닝 방식으로 만든 해시 테이블
void question3_print(int num3_array[])
{
    printf(" ****** Print : Chaining ******\n");
    printChainNode();
}


int main(void)
{  
    //해시테이블을 구성할 초기 값들.
    int input_data[8] = { 10,20,30,40,33,46,50,60 };
    //만들 해시테이블 num1 : 개방주소법 테이블, num2 : 이차원조사 테이블 , num3 : 체이닝 조사법 테이블
    int num1_array[13] = { 0, };
    int num2_array[13] = { 0, };
    int num3_array[13] = { 0, };




    //마지막 4개의 값을 찾아 index를 출력함.
    int find_key[4] = { 33,46,50,60 };
    printf("\n +++++++++ find index of key = {33,46,50,60} +++++++++\n");
    printf("\n --------- find index : Open Addressing ---------\n");
    question1(num1_array, input_data, find_key);
    question2(num2_array, input_data, find_key);
    printf(" --------- find index : Chaining ---------\n");
    question3(num3_array, input_data , find_key);


    printf("-------------------------------------------------\n");
    printf(" +++++++++ print Whole Array +++++++++ \n\n");


    //해당하는 해시테이블들을 출력해줌.
    printf(" --------- print hash : Open Addressing ---------\n");
    question1_print(num1_array);
    question2_print(num2_array);
    printf(" --------- print hash : Chaining ---------\n");
    question3_print(num3_array);



    return 0;
}
