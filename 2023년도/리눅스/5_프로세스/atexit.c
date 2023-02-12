#include <stdio.h>
#include <stdlib.h>

static void ssu_exit1 (void);
static void ssu_exit2 (void);

int main (void)
{
    if(atexit(ssu_exit2) != 0)
    {
        fprintf(stderr, "atexit Error : ssu_exit2\n");
        exit(1);
    }

    if(atexit(ssu_exit1) != 0)
    {
        fprintf(stderr, "atexit Error : ssu_exit1\n");
        exit(1);
    }

    printf("done\n");
    exit(0);
}


static void ssu_exit1(void)
{
    printf("SSU_EXIT1 handler!\n");
}

static void ssu_exit2(void)
{
    printf("SSU_EXIT2 handler!\n");
}
