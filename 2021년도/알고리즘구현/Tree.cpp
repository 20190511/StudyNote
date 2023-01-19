#include <iostream>
#include <string>
using namespace std;
/*
	Æ®¸® ±âº»±¸Á¶:
*/



class Node {
public:
	int data;
	Node *left;
	Node *right;
	
	Node(int value = 0)
	{
		this->data = value;
		this->left = NULL;
		this->right = NULL;
	}
};


class Tree {
public:
	Node *root;
	Tree (int value = 0)
	{
		this->root = new Node();
	}
	
	void preOrder (Node *node = root)
	{
		if(root != NULL)
		{
			cout<<root->data <<" ";
			preOrder(root->leftChild);
			preOrder(root->rightChild);
		}
	}
	
	
	void inOrder (Node *node = root)
	{
		if(root != NULL)
		{
			inOrder(root->leftChild);
			cout<<root->data<<" ";
			inOrder(root->rightChild);
		}
	}
	
	
	void postOrder (Node *node = root)
	{
		if(root != NULL)
		{
			postOrder(root->leftChild);
			postOrder(root->rightChild);
			cout<<root->data<<" ");
		}
	}

	
	void justInsert (int value)
	{
		Node *rememberRoot = root;
		if(root == NULL)
		{
			root->data = value;
			root->left = NULL;
			root->right = NULL;
		}
		else
		{
			while(1)
			{
				if(root->left == NULL)
				{
					
				}
				else if(root->right == NULL)
				{
					
				}
				else
				{
					
				}
			}
		}
	}	
};







int main(void)
{
	
	
	return  1;
}
