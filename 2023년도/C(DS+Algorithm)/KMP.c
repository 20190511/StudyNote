#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** KMP : 기본 전제 : 접미사 + 접두사 각 자리별 일치하는 접두==접미의 최대길이를 저장해뒀다가
 *  실제로 돌려봤을 때, 같은문자를 넘기다가, 다른 문자가나왔을 때 해당 문자 이전의 최대로 같은 길이만큼 패턴을 옮기고싶은데
 *  패턴에도 접미==접두 같은 문자가 있을 수 있으니 kmp_poattern에 해당하는 문자만큼 땡겨주겠다
 *  <ex>
 *  : 해당위치에서 최대로 같은 문자의 개수는 6개고, f에서 불일치했으니, 패턴 문자를 f에서 다시시작하고 싶은데 
 * ★ 접두문자열 == 접미문자열 만큼 문자를 땡겨줘야된다. 
 *  (search 구조에서 S='abcdabcdabd' 의 경우 첫타에 안맞다고 해서 abcdabc 의 c위치에서 다시 시작해버리면 같은 문자인데도 못찾는다...)
 *  f (search[6]) 이전의 배열위치값은 search[5] 에서 얼마만큼 땡겨야되는지 보아하니 lp[5] = 2 다
 *  2만큼 땡겨보자!         : 
 *  lp[5] = 
 *  
 *  <ex>
 *  abc abcdab abcdabcdabde
 *      abcdabd
 *            abcdabd           //에서 2만큼 오른쪽으로 땡겨야죠
 *          abcdabd             //여기서부터 시작하면된다.
 * 
 *  그리고 이미 앞의 ab부분은 체크되었으니까 lp[5]=2 부분까지 기록해두고 cdabd 부분만 체크해보면 된다.
*/
int* kmp_pattern (char *str);

int* kmp (char *strs, char *pat)
{
    int* lp = kmp_pattern(pat);

    int i = 0; /*strs(찾으려는 문자열) 포인터*/
    int j = 0; /*pat (패턴) 포인터*/
    int count = 0; /*i까지위치의 일치하는 개수*/

    int *result = (int*)malloc(sizeof(int)*strlen(strs));
    int result_count = 0;
    for (int k = 0 ; k < strlen(strs) ; k++)
        result[k] = -1;

    for (i = 0 ; i < strlen(strs) ; i++)
    {
        while(j>0 && strs[i] != pat[j]) /*j (pat 포인터) 를 먼저 설정해주고!*/
        {
            j = lp[j-1];
        }
        
        if (strs[i] == pat[j])
            j++;
        
        /* 같으면 Back만 해주면됨 while 안돌려줘도됨 : 뒤틀린위치에서 시작하는 것이아니기 때문*/
        if (j == strlen(pat))
        {
            result[result_count++] = i-j+1;
            j = lp[j-1];
        }
        

    }
    return result;
}

int* kmp_pattern (char *str) 
{
    int pat_len = strlen(str);
    int *lp = (int*)calloc(pat_len, sizeof(int));
    int i = 1;
    int j = 0;
    lp[0] = 0;
    /*같으면 j이동 다르면 j위치 리셋.*/
    while (i < pat_len)
    {
        if(str[j] == str[i])
        {
            lp[i] = j+1;
            j++;
        }   
        else
            j = 0;
        i++;
    }
    return lp;
}


int main(void)
{
    char *strs =  "abc abcdab abcdabcdabde\0";
    char *pat = "abcdabd";

    char *strs2 = "ababcabababcababaac";
    char *pat2 = "ababc";
    int *res = kmp(strs2,pat2);
    
    for (int i = 0 ; res[i] != -1 ; i++)
    {
        printf("%d ", res[i]);
    }
    printf("\n");



}
