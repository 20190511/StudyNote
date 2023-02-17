#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_INDEX      5
#define DATA_COUNT      10
#define NAME_SIZE       15
typedef struct DataSet {
    int key;
    char name[NAME_SIZE];
    char *data[DATA_COUNT];
    int data_count;
    DataSet *next; /*chaining 연결*/
}DataSet;

DataSet* initData (int key, char* name)
{
    DataSet* newData = (DataSet*)malloc(sizeof(DataSet));
    memset(newData->name, '\0', NAME_SIZE);
    strcpy(newData->name, name);
    newData->key = key;
    for (int i = 0 ; i < DATA_COUNT ; i++)
        newData->data[i] = '\0';
    newData->data_count = 0;
    newData->next = NULL; /*다음꺼는 마지막에 체이닝으로 달리는 형식*/
    return newData;
}

typedef struct Hash {
    DataSet *hashtable_front [HASH_INDEX]; /*해시테이블에 역을 예정.*/
    DataSet *hashtable_back [HASH_INDEX];
    int count_table[HASH_INDEX];
}Hash;


void initHash(Hash *hash)
{
    for (int i = 0 ;i < HASH_INDEX ; i++)
    {
        hash->hashtable_front[i] = NULL;
        hash->hashtable_back[i] = NULL;
        hash->count_table[i] = 0;
    }
}

/*삽입시간 O(1)*/
void push_data (Hash *hash, int key, char* name)
{
    DataSet *newNode = initData(key, name);
    int index = newNode->key % 5; /*아주 간결한 해시알고리즘*/
    
    if (hash->count_table[index] == 0) /*Hash table이 비어있으면 삽입*/
    {
        hash->hashtable_back[index] = hash->hashtable_back[index] = newNode;
        hash->count_table[index]++;
    }
    else
    {
        hash->hashtable_back[index]->next = newNode;
        hash->count_table[index]++;
    }
}

void print_DataSet(DataSet* data)
{
    printf("%s is key is %d\n",data->name, data->key);
    for (int i = 0 ; i < data->data_count ; i++)
    {
        printf("---> the data[%d] is %-15s\n", i, data->data[i]);
    }
}
int main(void)
{
    Hash h;
    initHash(&h);
    //print_DataSet(initData(256, "Junhyeong"));
    return 0;
}
