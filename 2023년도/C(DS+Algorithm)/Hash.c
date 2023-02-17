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
    DataSet *hashtable[HASH_INDEX]; /*해시테이블에 역을 예정.*/
    DataSet *headers [HASH_INDEX];
    int count_table[HASH_INDEX];
}Hash;


void initHash(Hash *hash)
{
    for (int i = 0 ;i < HASH_INDEX ; i++)
    {
        hash->hashtable[i] = NULL;
        hash->count_table[i] = 0;
    }
}

void push_data (Hash *hash, int key, char* name)
{
    DataSet *news = initData(key, name);
    int index = news->key % 5; /*아주 간결한 해시알고리즘*/
    
    if (hash->count_table[index] == 0) /*Hash table이 비어있으면 삽입*/
    {
        
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
