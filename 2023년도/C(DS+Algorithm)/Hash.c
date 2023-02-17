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
    struct DataSet *next; /*chaining 연결*/
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
        hash->hashtable_back[index] = hash->hashtable_front[index] = newNode;
        hash->count_table[index]++;
    }
    else
    {
        hash->hashtable_back[index]->next = newNode;
        hash->hashtable_back[index] = newNode;        
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

void print_hash(Hash *hash)
{
    DataSet* originals[HASH_INDEX]; /*header기억*/
    for (int i = 0 ; i < HASH_INDEX ;i++)
        originals[i] = hash->hashtable_front[i];

        printf("----   ----------------------   ---- \n");
    for (int i = 0 ; i < HASH_INDEX ; i++)
    {
        printf("|---    hash table index [%d]   ----| \n", i+1);
        for (int j = 0 ; j < hash->count_table[i] ; j++)
        {
            printf("| %15s 's key is %6d |\n", hash->hashtable_front[i]->name, hash->hashtable_front[i]->key);
            hash->hashtable_front[i] = hash->hashtable_front[i]->next;
        }
        printf("|----------------------------------|\n");
    }
        printf("----   ----------------------   ---- \n\n\n");
    for (int i = 0 ; i < HASH_INDEX ;i++)
        hash->hashtable_front[i] = originals[i];
}

void delete (Hash* hash, int key, char* name)
{
    int index = key%5 ; /*해시 알고리즘*/
    DataSet* delete_node;

    if (hash->count_table[index] == 0)
    {
        printf("This key is Wront!\n");
        return;
    }
    else if (strcmp(hash->hashtable_front[index]->name, name) == 0) /*strcmp는 같으면 0이다!*/
    {
        delete_node = hash->hashtable_front[index];
        printf("(f)%s' key(%d) will be deleted!\n",hash->hashtable_front[index]->name,hash->hashtable_front[index]->key);
        hash->hashtable_front[index] = hash->hashtable_front[index]->next;
        hash->count_table[index]--;
        free(delete_node);
        return;
    }
    else
    {
        DataSet *original = hash->hashtable_front[index];
        DataSet *parent = hash->hashtable_front[index];
        for (int i = 0 ; i < hash->count_table[index] ; i++)
        {
            if (strcmp(hash->hashtable_front[index]->name, name) == 0) /*strcmp는 같으면 0이다!*/
            {
                printf("%s' key(%d) will be deleted!\n",hash->hashtable_front[index]->name,hash->hashtable_front[index]->key);
                delete_node = hash->hashtable_front[index];
                parent->next = parent->next->next;
                hash->count_table[index]--;
                free(delete_node);
                hash->hashtable_front[index] = original;
                return;
            }
            parent = hash->hashtable_front[index];
            hash->hashtable_front[index] = hash->hashtable_front[index]->next;
        }
        hash->hashtable_front[index] = original;
    }
    printf("Your node %s and key(%d) is not listed in this hash\n", name, key);
}

void Data_push (Hash *hash, int key, char *name, char* data)
{
    int index = key%5;
    DataSet *original = hash->hashtable_front[index];
    for (int i = 0 ; i < hash->count_table[i] ; i++)
    {
        if (strcmp(hash->hashtable_front[index]->name, name) == 0)
        {
            if (hash->hashtable_front[index]->data_count == DATA_COUNT)
            {
                printf("Data POOL is FULL, Can't push this data %s\n", data);
                hash->hashtable_front[index] = original;
                return;
            }
            else
            {
                printf("Datapush : %s in index %d\n", data, hash->hashtable_front[index]->data_count);
                /*char* data[index] 에 char* data 타입을 넣는 경우, 포인터를 사용해서 변경하기 때문에 포인터를 바로넣어주면된다.*/
                hash->hashtable_front[index]->data[hash->hashtable_front[index]->data_count++] = data;
                hash->hashtable_front[index] = original;
                return;
            }
        }
        hash->hashtable_front[index] = hash->hashtable_front[index]->next;
    }
    hash->hashtable_front[index] = original;
    printf("%s key(%d) is not listed in these hash\n", name, key);
    return;
}


int main(void)
{
    Hash h;
    initHash(&h);
    push_data(&h, 432, "junhyeong");
    push_data(&h, 4432, "koi");
    push_data(&h, 532, "Doris");

    push_data(&h, 555, "soso");
    print_hash(&h);
    delete(&h, 532, "Doris");
    print_hash(&h);
    Data_push(&h, 432, "junhyeong", "Pohang");
    //print_DataSet(initData(256, "Junhyeong"));
    return 0;
}

/*

----   ----------------------   ---- 
|---    hash table index [1]   ----| 
|            soso 's key is    555 |
|----------------------------------|
|---    hash table index [2]   ----| 
|----------------------------------|
|---    hash table index [3]   ----| 
|       junhyeong 's key is    432 |
|             koi 's key is   4432 |
|           Doris 's key is    532 |
|----------------------------------|
|---    hash table index [4]   ----| 
|----------------------------------|
|---    hash table index [5]   ----| 
|----------------------------------|
----   ----------------------   ---- 


Doris' key(532) will be deleted!
----   ----------------------   ---- 
|---    hash table index [1]   ----| 
|            soso 's key is    555 |
|----------------------------------|
|---    hash table index [2]   ----| 
|----------------------------------|
|---    hash table index [3]   ----| 
|       junhyeong 's key is    432 |
|             koi 's key is   4432 |
|----------------------------------|
|---    hash table index [4]   ----| 
|----------------------------------|
|---    hash table index [5]   ----| 
|----------------------------------|
----   ----------------------   ---- 


Datapush : Pohang in index 0
*/
