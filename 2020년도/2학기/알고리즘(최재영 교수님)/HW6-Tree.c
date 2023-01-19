#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _TreeSet {
    int rank; //rank = 높이
    char data;
    struct _Set* link;
}TreeSet;

//일단 자기 자신을 가리키는 새 노드 생성
TreeSet* makeSet(char value)
{
    TreeSet* newNode = (TreeSet*)malloc(sizeof(TreeSet));
    newNode->data = value;
    newNode->link = newNode;
    newNode->rank = 0;
    return newNode;
}

//Find
TreeSet* findSet(TreeSet* root)
{
    if (root->link == root)
        return root;
    else
        findSet(root->link);
}

//Set 합치기
void treeUnion(TreeSet* firstTree, TreeSet* secondTree)
{
    TreeSet* xTreeRoot = findSet(firstTree);
    TreeSet* yTreeRoot = findSet(secondTree);
    //rank가 작은 트리의 루트의 링크를 큰 트리의 루트에 연결
    if (xTreeRoot->rank > yTreeRoot->rank)
    {
        yTreeRoot->link = xTreeRoot;
        printf("%c --> %c \n",yTreeRoot->data , xTreeRoot->data);
    }
    else
    {
        //xRank <= yRank인 경우 -> x링크를 y링크로 주고 y링크 1 추가해줌. (즉, y링크가 루트가됨)
        xTreeRoot->link = yTreeRoot;
        printf("%c --> %c \n", xTreeRoot->data, yTreeRoot->data );
        if (xTreeRoot->rank == yTreeRoot->rank)
            yTreeRoot->rank++; //rank가 같아짐을 방지
    }
}

TreeSet* question1_1()
{
    //세팅과정
    printf(" +++++++++ question1: (a) +++++++++\n");
    TreeSet* question1[3];
    question1[0] = makeSet('a');
    question1[1] = makeSet('b');
    treeUnion(question1[1], question1[0]);
    question1[2] = makeSet('c');

    //문제의 본론
    treeUnion(question1[2], question1[1]);
    printf("\n");
    return question1[0];
}
TreeSet* question1_2()
{
    //세팅과정
    printf(" +++++++++ question1 : (b) +++++++++\n");
    TreeSet* question2[6];
    question2[0] = makeSet('d');
    question2[1] = makeSet('e');
    treeUnion(question2[1], question2[0]);
    question2[2] = makeSet('f');
    question2[3] = makeSet('g');
    treeUnion(question2[3], question2[2]);
    question2[4] = makeSet('h');
    treeUnion(question2[2], question2[4]);
    question2[5] = makeSet('i');
    treeUnion(question2[2], question2[5]);
    
    //문제의 본론
    treeUnion(question2[0], question2[3]);
    printf("\n");
    return question2[0];
}

void question_final()
{
    TreeSet* finalTree[2];
    finalTree[0] = question1_1();
    finalTree[1] = question1_2();
    printf(" +++++++++ question1 : whole Union +++++++++\n");
    treeUnion(finalTree[0], finalTree[1]);
    printf("\n");
}

int main(void)
{
    question_final();
    return 0;
}
