#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GCCBUF  30
char* gccbuf [15];

int gcc (char *objects)
{
    FILE *gcc_fp = fopen("MakeFile", "w+");

    int count = 0;
    gccbuf[count] = (char*)malloc(strlen("CC=gcc\n")* sizeof(char));
    strcpy(gccbuf[count++], "CC=gcc\n");

    gccbuf[count] = (char*)malloc(strlen("CFLAGS=-g -Wall\n")* sizeof(char));
    strcpy(gccbuf[count++], "CFLAGS=-g -Wall\n");

    gccbuf[count] = (char*)malloc(strlen("TARGET=a.out\n")* sizeof(char));
    strcpy(gccbuf[count++], "TARGET=a.out\n");
    
    char objs[GCCBUF];
    sprintf(objs, "OBJS=%s\n", objects);
    gccbuf[count] = (char*)malloc(strlen(objs)* sizeof(char));
    strcpy(gccbuf[count++], objs);

    
    gccbuf[count] = (char*)malloc(strlen("\nall: $(TARGET)\n")* sizeof(char));
    strcpy(gccbuf[count++], "\nall: $(TARGET)\n");

    gccbuf[count] = (char*)malloc(strlen("\n$(TARGET): $(OBJS)\n")* sizeof(char));
    strcpy(gccbuf[count++], "\n$(TARGET): $(OBJS)\n");
    
    gccbuf[count] = (char*)malloc(strlen("\t$(CC) -o $@ $(OBJS)")* sizeof(char));
    strcpy(gccbuf[count++], "\t$(CC) -o $@ $(OBJS)");


    for (int i = 0 ; i  < count ; i++)
    {
        if(fputs(gccbuf[i], gcc_fp)  == -1)
        {
            fprintf(stderr, "Fputs Error\n");
            exit(1);
        }
    }
    fclose(gcc_fp);
    return count;
}


int main(void)
{
    int count = gcc("test.c");
    for (int i = 0 ; i < count ; i++)
        printf("%s", gccbuf[i]);

    return 0;
}
