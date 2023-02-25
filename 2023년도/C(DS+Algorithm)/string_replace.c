

int kmp (char* origin, char* target)
{   
    int tar_len = strlen(target);
    int origin_len = strlen(origin);
    if (tar_len <= 1)
    {
        for (int i = 0 ; i < origin_len ; i++)
        {
            if (origin[i] == target[0])
                return i;
        }
        return -1;
    }
    else
    {
        int* target_array = (int*)calloc(tar_len, sizeof(int));
        int tar_j = 0;
        for (int i = 1 ; i < tar_len ; i++)
        {
            if (target[tar_j] == target[i])
            {
                target_array[i] = tar_j+1;
                tar_j++;
            }
            else
                tar_j = 0;
        }

        tar_j = 0;
        int total_check = 0;
        for (int i = 0 ; i < origin_len ; i++)
        {

            while (tar_j > 0 && target[tar_j] != origin[i])
            {
                tar_j = target_array[tar_j-1];
                total_check = tar_j;
            }
            
            if (target[tar_j] == origin[i])
            {
                tar_j++;
                total_check++;
            }

            if (total_check == tar_len)
            {
                printf("Same String idx is %d\n", i-tar_len+1);
                return  i-tar_len+1;
            }
        }

        free(target_array);
        return -1;
    }

}
/** 
 *  original 문자열의 rep_before 을 rep_after를 cnt만큼 바꿔줌
 *  cnt == 0 이면 모든 문자열을 교체
 *
 * original 의 rep_before 을 rep_after로 교체 
 *  kmp로 일치 문자열 찾기
 *  ptr = 일치문자열 + 찾을 문자열길이 해서 뒷부분 복사
 *  원본 -> 바꿀 문자열 바꾸기
 *  붙여넣기
*/
char* replace (char* original, char* rep_before, char* rep_after, int cnt)
{
    char* temp_char = (char*)malloc(strlen(original));
    strcpy(temp_char,original);
    int rep_cnt = cnt == 0 ? strlen(original) : cnt; //삼항연산이용.
    for (int i = 0 ; i < rep_cnt ; i++)
    {
        char* ptr = temp_char;
        int original_len = strlen(temp_char);
        int replace_idx = kmp(temp_char, rep_before);
        if (replace_idx == -1)
            return temp_char;
        ptr += replace_idx;
        
        //후방 문자열 복사.
        int rear_string = replace_idx+strlen(rep_before);
        char *temp_rear;
        int rear_cnt;
        if (rear_string <= original_len)
        {
            rear_cnt = original_len - rear_string;
            temp_rear = (char*)malloc(rear_cnt+1);
            memcpy(temp_rear, ptr+strlen(rep_before), rear_cnt);
            temp_rear[rear_cnt] = '\0';
        }

        strcpy(ptr, rep_after);
        ptr += strlen(rep_after);
        *ptr = '\0';
        strcat(temp_char, temp_rear);
    }

    return temp_char;
}
