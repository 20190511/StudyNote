#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    printf("This is the Orignal Program\n");
    execl("./execl", "execl", "ssu_test1", "ssu_test2", "ssu_test3", (char*)NULL);
    printf("Never Print this line\n");
    exit(0);
}
