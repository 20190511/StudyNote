/* 스택은 연결리스트 형태의 pop과 동일함*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TRANS(_INT) #_INT
#define BUFFERS   256
#define FILE_NAME "test.txt"
char ints [15];

typedef struct Node {
    int data;
    struct Node *next;
}Node;

typedef struct Stack
{
    Node *front;
    int count;
}Stack;

void initStack (Stack *stack)
{
    stack->front = NULL;
    stack->count = 0;
}

int isEmpty(Stack *stack)
{   
    return stack->count == 0;
}

void push(Stack *stack, int data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->data = data;
    if (isEmpty(stack))
    {
        stack->front = newNode;
        stack->count++;
    }
    else
    {
        newNode->next = stack->front;
        stack->front = newNode;
        stack->count++;
    }
}

int pop(Stack *stack)
{
    if (stack->count <= 0)
    {
        printf("Can't POP : This Stack is Empty!\n");
        return -1;
    }

    Node *delNode;
    int return_val;
    if (stack->count == 1)
    {
        delNode = stack->front;
        return_val = delNode->data;
        stack->front = NULL;
        stack->count--;
        free(delNode);
        return return_val;
    }
    else
    {
        delNode = stack->front;
        return_val = delNode->data;
        stack->front = stack->front->next;
        free(delNode);
        stack->count--;
        return return_val;
    }
}


void print_node(Stack *stack)
{
    Node *orignal = stack->front;
    if (stack->count == 0)
    {
        printf("CAN'T PRINT : Null Stack!\n");
        return;
    }

    int i;
    for (i=0 ; i<stack->count ; i++)
    {
        printf("[back] ");
        printf("%d", stack->front->data);
        stack->front = stack->front->next;
        if (stack->front)
            printf("<-");
        else
            printf(" [front], {count=%d}\n",stack->count);
    }
    stack->front = orignal;
}


char buf [BUFFERS];
void print_file(Stack *stack, int fd)
{
    Node *orignal = stack->front;
    if (stack->count == 0)
    {
        strcpy(buf, "CAN'T PRINT : Null Stack!\n");
        int size = write(fd, buf, strlen(buf));
        if (size != strlen(buf))
        {fprintf(stderr, "Write Error\n"); exit(1);}
        return;
    }

    int i;

    strcpy(buf, "[back] ");
    for (i=0 ; i<stack->count ; i++)
    {
        int temp = stack->front->data;
        sprintf(ints,"%d", temp);
        strcat(buf, ints);
        stack->front = stack->front->next;
        if (stack->front)
            strcat(buf, "<-");
        else
        {
            strcat(buf, " [front], {count=");
            sprintf(ints, "%d}\n", stack->count);
            strcat(buf, ints);
        }
    }
    int size = write(fd, buf, strlen(buf));
    if(size != strlen(buf))
    {fprintf(stderr, "Write Error2\n"); exit(1);}
    stack->front = orignal;
    return;
}


void execute(int fd)
{
    Stack s;
    initStack(&s);
    push(&s, 5);
    push(&s, 1);
    push(&s, 3);
    push(&s, 9);
    push(&s, 11);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    pop(&s);
    print_file(&s, fd);
    fsync(fd);
}


int main(void)
{
    int fd;
    if ((fd = open(FILE_NAME, O_RDWR | O_APPEND, 0666)) < 0)
    {
        fprintf(stderr, "Open Error\n");
        exit(1);
    }
    execute(fd);
    return 0;
}
