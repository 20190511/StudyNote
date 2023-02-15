/* 스택은 연결리스트 형태의 pop과 동일함*/
#include <stdio.h>
#include <stdlib.h>

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

    printf("Print start! \n[BACK] ");
    int i;
    for (i=0 ; i<stack->count ; i++)
    {
        printf("%d", stack->front->data);
        stack->front = stack->front->next;
        if (stack->front)
            printf("<-");
        else
            printf(" [front]\n");
    }
    printf("Print Finished\n");
    stack->front = orignal;
}

int main(void)
{
    Stack s;
    initStack(&s);
    push(&s, 5);
    push(&s, 1);
    push(&s, 3);
    push(&s, 9);
    push(&s, 11);
    print_node(&s);
    pop(&s);
    print_node(&s);
    pop(&s);
    print_node(&s);
    pop(&s);
    print_node(&s);
    pop(&s);
    print_node(&s);
    pop(&s);
    print_node(&s);
    pop(&s);
    print_node(&s);
}
