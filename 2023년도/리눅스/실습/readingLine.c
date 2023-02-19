""" getc 를 이용해서 파일 데이터 1 줄씩기읽어와 버퍼에 저장해두기 """
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE      100
#define INDEX_SIZE      20
#define FILENAME    "test.c"

typedef struct {
    char strbuf[256];
    int offset;
    int length;
}Buf;

void reading(char *filename)
{
    
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "FileOpen Error!\n");
        exit(1);
    }
    Buf buffer[BUFFER_SIZE]; 
    buffer[0].length=0;
    buffer[0].offset=0;
    
    int count = 0;
    int c;
    while((c = fgetc(fp))>0 && !feof(fp))
    {
        
        if (count >= BUFFER_SIZE)
        {
            fprintf(stderr, "Capcity excess!\n");
            exit(1);
        }
        if (c<0 && ferror(fp))
        {
            fprintf(stderr, "fgetc Error!\n");
            exit(1);
        }

        if (c == '\n')
        {
            buffer[count].strbuf[buffer[count].length] = 0;
            puts(buffer[count].strbuf);
            buffer[count+1].length = 0;
            buffer[count+1].offset = buffer[count].length;
            count++;
        }
        else
        {
            buffer[count].strbuf[buffer[count].length] = c;
            buffer[count].length++;
        }
    } 
    puts(buffer[count].strbuf);
    printf("count of file lines is %d\n", count+1);
    fclose(fp);
} 

int main(int argc, char* argv[])
{
    char print_cwd[256];
    getcwd(print_cwd, 256);
    printf("this file path is : %s\n", print_cwd);
    reading(argv[1]);
    exit(0);
}
