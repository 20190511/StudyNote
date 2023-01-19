#include <iostream>

using namespace std;

class Node {
public:
	int data;
	Node* left;
	Node* right;

	Node()
	{
		this->data = 0;
		this->left = NULL;
		this->right = NULL;
	}
};


class List {
public:
	Node* value;
	List* link;

	List()
	{
		value = new Node();
		this->link = NULL;
	}
	void insert(int value);
	Node* pop();

	void print();
};


void List::insert(int value)
{
	List* newNode = new List();
	newNode->value->data = value;
	newNode->link = link;

	link = newNode;
}

Node* List::pop()
{
	List* tempList = new List();
	tempList->link = link;

	link = link->link;

	return tempList->value;
}


void List::print()
{
	List* p = link;
	while (p != NULL)
	{
		cout << p->value->data;
		if (p->link != NULL)
			cout << "->";
		p = p->link;
	}
	cout << endl;
}
