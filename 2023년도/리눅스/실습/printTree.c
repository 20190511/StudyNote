#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define MAX_NAME     255
#define MAX_UNITS   30

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
            printf("%s is directory\n",path_cat);
            node->dir_next[node->dir_count] = newNode;
            node->dir_count++;
        }
        else
        {
            if (node->max_count[1] == node->other_count+1)
                expand_other_next(node);
            printf("%s is other file\n",path_cat);
            node->other_next[node->other_count] = newNode;
            node->other_count++;
        }

        

        memset(path_cat, '\0', MAX_NAME*2);
        dir = readdir(dp_root);
    }
    closedir(dp_root);
}

void dp_call(int depth)
{
    Folder *folder = init_folder("/");
    root_exploror(folder->root);

    Node *orignal_root = folder->root;
    for (int i = 1 ; i < depth ; i++)
    {
        printf("root dir_count = %d\n", folder->root->dir_count);
        for (int k = 0 ; k < folder->root->dir_count ; k++)
        {
            Node* cur = folder->root->dir_next[k];
            if (access(cur->name, R_OK) != 0) /* 접근 권한 확인*/
            {
                printf("%s can't exploror\n", cur->name);
                continue;
            }
            root_exploror(cur);
            printf("%s 's unit count is %d + %d = %d\n",cur->name, cur->dir_count, cur->other_count, cur->dir_count+cur->other_count);
        }  
    }
}

int main(void)
{
    dp_call(2);
    return 0;
}
