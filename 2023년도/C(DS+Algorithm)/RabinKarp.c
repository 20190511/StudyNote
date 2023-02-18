/** H[i] = S[i] 2^m-1 + S[i+1] 2^m-2 + ... S[i+m-1] 2^0 //점점 작은수를 곱하는식
 *  H[i+1] = S[i+1] 2^m-1 + .. + S[i+m-1]*2 + S[i+m]
 *         = 2H[i] - S[i]*2^m + S[i+m]
 *  m 은 pattern 글자의 크기
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int hashing (char *str, int size)
{
    int sum = 0;
    for (int i = 0 ; i <size ; i++)
    {
        sum += (str[i]) * (1<<(size-i-1));
    }
    return sum;
}

/** resallocs(&result, &result)
 * 
*/
void reallocs (int** result, int* size)
{
    int original = *size;
    *size *= 2;
    *result = (int*)realloc(*result, sizeof(int)*(*size));
    for (int i = original ; i < *size ; i++ )
        (*result)[i] = -1;

}

int check_hash(char str[], char pat[], int start_idx)
{
    int pat_len = strlen(pat);
    int check_num = 1;
    int pat_idx = 0;
    for (int i = start_idx ; i < start_idx+pat_len ; i++)
    {
        if (str[i] != pat[pat_idx++])
        {
            check_num = 0;
            return check_num;
        }
    }
    return check_num;
}

int* RabinKarp (char str[], char pat[])
{
    int *index = (int*)malloc(sizeof(int) * strlen(str));
    for (int i = 0 ; i < strlen(pat)-1 ; i++)
        index[i] = 0;
    
    int key_hash = hashing(pat, strlen(pat));
    index[0] = hashing(str, strlen(pat));    

    int result_maxsize = 5;
    int *result = (int*)malloc(sizeof(int) * result_maxsize);
    for (int k = 0 ; k < result_maxsize ; k++)
        result[k] = -1;

    int result_count = 0;
    for (int j = 0 ; j < strlen(str) - strlen(pat)+1 ; j++)
    {
        if(j != 0)
            index[j] = index[j-1]*2 - (str[j-1])*(1<<strlen(pat)) + str[j+strlen(pat)-1];
        if (index[j] == key_hash)
        {
            if (result_maxsize == result_count)
                reallocs(&result, &result_maxsize);
            if (check_hash(str,pat,j))
                result[result_count++] = j;
        }
    }
    return result;
}

int main(void)
{
    char str1[] = "AABDDCDAASDAAASDASFAAASDABD";
    char str2[] = "AA";
    hashing(str2, 3);
    int* idx = RabinKarp(str1, str2);
    for (int i = 0 ; i < 1000000; i++)
    {
        if (idx[i] != -1)
        {
            printf("%d ", idx[i]);
        }
        else
            break;
    }
    printf("\n");
    return 0;
}
/*

0 7 11 12 19 20

*/
