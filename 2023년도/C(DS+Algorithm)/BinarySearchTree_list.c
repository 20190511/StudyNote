#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    int dup_count; /*중복되는 값 개수 : dup_count가 0보다 크면 삭제하더라도 가만히 있음.*/
    int data;
}Node;

typedef struct Tree{
    Node *root;
    int count;
}Tree;

void initTree(Tree *tree)
{
    tree->root = NULL;
    tree->count = 0;
}

void initNode (Node *node)
{
    node->prev = node->next = NULL;
    node->dup_count = 0;
}

Node* search_tree (Tree* tree, int key)
{
    if (tree->root == NULL)
        return NULL;
    Node *original = tree->root;
    Node *return_node;
    while (1)
    {   
        /* 같으면 성공 다르면 실패.*/
        if (tree->root->data == key)
        {
            return_node = tree->root;
            tree->root = original;
            return return_node;
        }
        else if (tree->root->data < key)
        {
            if (tree->root->next == NULL)
            {
                return_node = tree->root;
                tree->root = original;
                return return_node;
            }
            tree->root = tree->root->next;
        }
        else if (tree->root->data > key)
        {
            if (tree->root->prev == NULL)
            {
                return_node = tree->root;
                tree->root = original;
                return return_node;
            }
            tree->root = tree->root->prev;
        }
    }

}

int search(Tree *tree, int key)
{
    Node *node = search_tree(tree, key);
    if (node == NULL)
    {
        printf("Null Tree\n");
        return 0;
    }
    return node->data == key;
}

void push(Tree *tree, int data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    initNode(newNode);
    newNode->data = data;
    
    Node *find_parent = search_tree(tree, data);
    if (tree->count == 0)
    {
        tree->root = newNode;
        tree->count++;
        return;
    }
    
    if (find_parent->data != data)
    {
        if (find_parent->data < data)
            find_parent->next = newNode;
        if (find_parent->data > data)
            find_parent->prev = newNode;
        tree->count++;
        return;
    }
    else
    {
        printf("Common Key = %d\n",data);
        find_parent->dup_count++;
        tree->count++;
        return;
    }
    
}

/*해당 위치에 바뀐 노드*/
Node* delete_process (Node *delnode)
{
    Node* dels = delnode;
    if (delnode->prev==NULL && delnode->next==NULL)
    {
        free(dels);
        return NULL;
    }
    if ((delnode->prev && !delnode->next) || (!delnode->prev && delnode->next))
    {
        Node* returnnode;
        if (delnode->prev)
        {
            returnnode = delnode->prev;
            free(dels);
            return returnnode;

        }
        else
        {
            returnnode = delnode->next;
            free(dels);
            return returnnode;

        }
    }
    else
    {
        Node *delsPrev = delnode->prev;
        Node *ret = delsPrev;
        if (ret->next == NULL) /*ret의 오른쪽 자식이 없는 경우*/
        {
            ret->next = delnode->next;
            free(dels);
            return ret;
        }

        for (ret = delsPrev ; ret->next->next!=NULL ; ret = ret->next );
        
        Node *newParent = ret->next;
        ret->next = newParent->prev; 

        newParent->prev = delnode->prev;
        newParent->next = delnode->next;
        free(dels);
        return newParent;
    }   
    
}

int delete_node (Tree *tree, int key)
{
    if (tree->root == NULL)
    {
        printf("Tree is Empty!\n");
        return -1;
    }

    else if (search(tree, key) == 0)
    {
        printf("the key is not in tree\n");
        return -1;
    }
    
    Node* original = tree->root;
    Node* parent = tree->root; /*부모를 기억해두기*/

    while(1) /*삭제할 노듭 부모 찾아야함*/
    {

        if (tree->root->data == key)
            break;
        else if (tree->root->data < key)
        {
            parent = tree->root;
            tree->root = tree->root->next;
            continue;
        }
        else if (tree->root->data > key)
        {
            parent = tree->root;
            tree->root = tree->root->prev;
            continue;
        }
    }

    if (parent->data == key) /*Root 노드의 경우*/
    {   
        int return_val = key;
        if(parent->dup_count > 0)
        {
            parent->dup_count--;
            tree->count--;
            return key;
        }
        tree->root = delete_process(tree->root);
        tree->count--;
        return return_val;
    }
    else if (parent->data < key)
    {
        int return_val = parent->next->data;
        if (parent->next->dup_count > 0)
        {
            parent->next->dup_count--;
            tree->count--;
            tree->root = original; /*Root 복귀*/
            return return_val;
        }
        parent->next = delete_process(parent->next);
        tree->count--;
        tree->root = original; /*Root 복귀*/
        return return_val;
    }
    else
    {
        int return_val = parent->prev->data;
        if (parent->prev->dup_count > 0)
        {
            parent->prev->dup_count--;
            tree->count--;
            tree->root = original; /*Root 복귀*/
            return return_val;
        }
        parent->prev = delete_process(parent->prev);
        tree->count--;
        tree->root = original; /*Root 복귀*/
        return return_val;
    }
}





 /*작은 노드부터 프린트*/
void print_tree(Node *node) /*root노드 넣을것*/
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    if (node->prev)
    {
        print_tree(node->prev);
    }
    if (node->next)
    {
        print_tree(node->next);
    }

}

void print_trees(Tree* tree)
{
    print_tree(tree->root);
    printf("\n");
}
int main(void)
{
    Tree t;
    initTree(&t);
    push(&t, 3);
    push(&t, 2);
    push(&t, 10);
    push(&t, 15);
    push(&t, 15);
    push(&t, 15);
    push(&t, 8);
    push(&t, 9);
    push(&t, 4);
    push(&t, 5);
    push(&t, 7);
    push(&t, 6);
    for (int i = 0 ; i < 12 ; i++)
    {
        delete_node(&t,t.root->data);
        print_trees(&t);
    }
    
}
