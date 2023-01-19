#include <iostream>
using namespace std;


class Node {
public:
	int data;
	Node *prev;
	Node *next;
	
	Node ()
	{
		this->data = 0;
		this->prev = this->next = NULL;
	}
};

class DoubleList {
public:
	int size;
	Node *head , *tail;
	
	
	DoubleList()
	{
		this->size = 0;
		this->head = this->tail = NULL;
	}
	
	
	void insert (int value)
	{
		
		Node *newNode = new Node();
		newNode->data = value;
		
		
		if(this->size == 0)
		{
			newNode->next = newNode->prev = NULL;
			head = tail = newNode;
		}
		else
		{
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
		
		//중요
		this->size++;
	}
	
	void insertReverse (int value)
	{
		Node *newNode = new Node();
		newNode->data = value;
		
		if(this->size == 0)
		{
			newNode->next = newNode->prev = NULL;
			head = tail = newNode;
		}
		else
		{
			newNode->prev = tail;
			tail->next = newNode;
			tail = newNode;
		}
		
		this->size++;
	}
	
	
	void deleteByIndex (int index)
	{
		if( index == 0 || index > this->size || this->size == 0)
			return;
			
		Node *originalHeadPosition = head;
		Node *tempNode;
		
		for (int i = 0 ; i < index-1 ; i++)
		{
			head = head->next;
		}
		
		//size == 1이면 그냥 삭제.
		if ( this->size == 1)
		{
			tempNode = head;
			head = tail = NULL;
			
			delete(tempNode);
			this->size--;
		}
		else if (index == 1)
		{
			tempNode = head;
			head = head->next;
			head->prev = NULL;  //안해줘도됨. 
			delete(tempNode);
			this->size--;
		}
		else if (index == this->size)
		{
			tempNode = head;
			head = head->prev;
			head->next = NULL;
			tail = head;          //tail을 head위치로 변환.
			head = originalHeadPosition;
			delete(tempNode);
			this->size--;
		}
		else //중간에 끼어있는경우.
		{
			tempNode = head;
			head->prev->next = head->next;
			head->next->prev = head->prev;
			
			head = originalHeadPosition;
			delete(tempNode);
			this->size--;
		}
	}
	
	void print()
	{
		Node *originalHeadPosition = head;   //Node로 생성해줘야됨. 
		
		while (head != NULL)
		{
			cout<<head->data<<"->";
			if(head->next == NULL)
			{
				cout<<endl;
			}
			head = head->next;
		}
		
		head = originalHeadPosition;
	}
};




int main (void)
{
	DoubleList exer;    //DoubleList *exer 아님  
	exer.insert(10);
	exer.insert(20);
	exer.insertReverse(30);
	
	exer.print(); 
	exer.deleteByIndex(2);
	exer.print();
	
	return 1;
}
