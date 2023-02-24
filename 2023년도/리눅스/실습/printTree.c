#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define MAX_NAME     255
#define MAX_UNITS   30

char *replaceAll(char *s, const char *olds, const char *news);

typedef struct Node
{
    struct Node** dir_next;
    struct Node** other_next;
    char name[MAX_NAME];
    int dir_count;
    int other_count;
    int max_count[2];
}Node;

typedef struct Folder
{
    Node *root;
    int root_count;
}Folder;

Node* node_call (char node_name[])
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->dir_next = (Node**)malloc(sizeof(Node*) * MAX_UNITS);
    node->other_next = (Node**)malloc(sizeof(Node*) * MAX_UNITS);
    memset(node->name, '\0', MAX_NAME);
    strcpy(node->name, node_name);
    for (int i = 0 ; i < MAX_UNITS ; i++)
    {
        node->dir_next[i] = NULL;
        node->other_next[i] = NULL;
    }
    node->dir_count = 0;
    node->other_count = 0;
    node->max_count[0] = MAX_UNITS;
    node->max_count[1] = MAX_UNITS;
    return node;
}

Folder* init_folder(char root_path[])
{
    Folder *folder = (Folder*)malloc(sizeof(Folder));
    Node *newNode = node_call(root_path);
    strcpy(newNode->name, root_path);
    folder->root = newNode;
    folder->root_count = 0;
    return folder;
}

void expand_dir_next(Node *node)
{
    int cur_count = node->max_count[0];
    cur_count *= 2;
    node->dir_next = realloc(node->dir_next, cur_count * sizeof(Node*));
    node->max_count[0] = cur_count;
}

void expand_other_next (Node *node)
{
    int cur_count = node->max_count[1];
    cur_count *= 2;
    node->other_next = realloc(node->other_next, cur_count * sizeof(Node*));
    node->max_count[1] = cur_count;
}

void root_exploror(Node *node)
{
    DIR *dp_root;
    struct dirent* dir;
    struct stat stat_buf;
    
    dp_root = opendir(node->name);
    dir = readdir(dp_root);
    char path [MAX_NAME];
    strcpy(path, node->name);
    char path_cat [MAX_NAME*2];

    while (dir != NULL)
    {
        strcpy(path_cat, path);
        if (strcmp(path, "/") != 0)
            strcat(path_cat,"/");
        strcat(path_cat, dir->d_name);
        Node* newNode = node_call(path_cat);
        
        if (strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name, "..")==0)
        {
            memset(path_cat, '\0', MAX_NAME*2);
            dir = readdir(dp_root);
            continue;
        }
        stat(path_cat, &stat_buf);
        if (S_ISDIR(stat_buf.st_mode))
        {
            if (node->max_count[0] == node->dir_count+1)
                expand_dir_next(node);
            //printf("%s is directory\n",path_cat);
            node->dir_next[node->dir_count] = newNode;
            node->dir_count++;
        }
        else
        {
            if (node->max_count[1] == node->other_count+1)
                expand_other_next(node);
            //printf("%s is other file\n",path_cat);
            node->other_next[node->other_count] = newNode;
            node->other_count++;
        }

        

        memset(path_cat, '\0', MAX_NAME*2);
        dir = readdir(dp_root);
    }
    closedir(dp_root);
}


void exploror (Node* node)
{
    for (int k = 0 ; k < node->dir_count ; k++)
        {
            Node* cur = node->dir_next[k];
            if (access(cur->name, R_OK) != 0) /* 접근 권한 확인*/
            {
                printf("%s can't exploror\n", cur->name);
                continue;
            }
            root_exploror(cur);
            printf("%s 's unit count is %d + %d = %d\n",cur->name, cur->dir_count, cur->other_count, cur->dir_count+cur->other_count);
        }  
}


void rceulsive_exploror (Node *node, int depth, int cur_depth)
{
    static char remember_past_path  [10][MAX_NAME];
    static FILE *fp;
    fp = fopen("print_text.txt","w");
    if (depth == cur_depth+1)
    {
        int node_dir_cnt = node->dir_count;
        for (int i = 0 ; i < node_dir_cnt; i++)
        {
            Node *cur_node = node->dir_next[i];
            /*
            if (access(cur_node->name, R_OK) != 0) // 접근 권한 확인
            {
                printf("%s can't exploror\n", cur_node->name);
                continue;
            }
            root_exploror(cur_node);
            int total_unit_size = cur_node->dir_count+cur_node->other_count;
            printf("%s 's unit count is %d + %d = %d\n",
            cur_node->name, cur_node->dir_count, cur_node->other_count, total_unit_size);
            char print_unit[MAX_NAME] = {'\0',};
            strcpy(print_unit,replaceAll(cur_node->name, "/", "|─────"));
            printf("%s\n", print_unit);
            */
            char *print_unit = cur_node->name;
            char token_val[10][MAX_NAME] = {'\0',};
            int cnt = 0;
            char* tmp_chars;
            tmp_chars = strtok(print_unit,"/");
            int root_change = 0;
            if (strcmp(remember_past_path[cnt], tmp_chars) == 0)
            {
                printf("│     ");
                fprintf(fp, "│     ");
                strcpy(token_val[cnt++],"");
                root_change += 1;
            }
            else
            {
                printf("└─────");
                fprintf(fp, "└─────");
                strcpy(remember_past_path[cnt], tmp_chars);
                strcpy(token_val[cnt++],tmp_chars);
            }

            while((tmp_chars = strtok(NULL,"/"))!= NULL)
            {
                if (strcmp(remember_past_path[cnt], tmp_chars) == 0)
                {
                    strcpy(token_val[cnt++], "");
                }
                else
                {
                    strcpy(remember_past_path[cnt], tmp_chars);
                    strcpy(token_val[cnt++], tmp_chars);
                }
            }
            
            for (int idx = 0 ; idx < cnt ; idx++)
            {
                printf("%-20s",token_val[idx]);
                fprintf(fp, "%-20s",token_val[idx]);
                if(idx+1 < cnt)
                {
                    if (strcmp(token_val[idx+1], "") == 0)
                    {
                        printf("│     ");
                        fprintf(fp, "│     ");
                    }
                    else
                    {
                        printf("├─────");
                        fprintf(fp, "├─────");
                    }
                }
            }
            printf("\n");
            fprintf(fp, "\n");
        }
    }
    else
    {
        for (int i = 0 ; i < node->dir_count ; i++)
        {
            Node *process = node->dir_next[i];
            if (access(process->name, R_OK) != 0) // 접근 권한 확인
            {
                //printf("%s can't exploror\n", process->name);
                continue;
            }
            root_exploror(process);
            rceulsive_exploror(process, depth, cur_depth+1);
        }
    }

}

char *replaceAll(char *s, const char *olds, const char *news) {
  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);


  if (newlen != oldlen) {
    for (i = 0; s[i] != '\0';) {
      if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
      else i++;
    }
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
    if (memcmp(s, olds, oldlen) == 0) {
      memcpy(sr, news, newlen);
      sr += newlen;
      s  += oldlen;
    } else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}

void print_rceulsive_exploror (Node *node, int depth, int cur_depth)
{
    if (depth == cur_depth)
    {
        int node_dir_cnt = node->dir_count;
        char print_unit[MAX_NAME] = {'\0',};
        strcpy(print_unit,replaceAll(node->name, "/", "|─────"));
        printf("%s\n", print_unit);
        
    }
    else
    {
        for (int i = 0 ; i < node->dir_count ; i++)
        {
            Node *process = node->dir_next[i];
            print_rceulsive_exploror(process, depth, cur_depth+1);
        }
    }

}



void print_tree(Folder *folder, int depth)
{
    Node *root = folder->root;
    print_rceulsive_exploror(root, depth, 0);
}

/** 해당 root_path 경로를 depth 깊이만큼 탐색하는 함수
 *  접근하지 못하는 경우에는 access(path, R_OK) 를 확인하여 걸러주면서 실행
 */
void dp_call(char root_path[], int depth)
{
    Folder *folder = init_folder(root_path);
    root_exploror(folder->root);

    Node *orignal_root = folder->root;
    rceulsive_exploror(folder->root, depth, 0);

    //print_tree(folder, depth);
}

//#define DEBUG
#ifdef DEBUG
int main(void)
{
    dp_call("/home",5);
    return 0;
}
#else
int main(int argc, char* argv[])
{
    dp_call(argv[1], atoi(argv[2]));
    return 0;
}
#endif
