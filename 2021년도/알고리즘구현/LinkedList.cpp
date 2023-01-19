#include <iostream>
#include <string>
using namespace std;

class Node {
public:
	int m;
	Node *link;
	
	Node (int a = 0){
		m = a;
		link = NULL;
	}
	
	//핵심 함수. 
	void insert(Node *newNode)
	{
		newNode->link = link;  //newNode의 link를 현재 주소로 지정. 
		link = newNode;        //새로운 주소값을 저장. 
	}
	
	void pop ()
	{
		if(link != NULL)
		{
			cout<<"remove value = "<<link->m<<endl;
			link = link->link; //링크를 이전으로 이동. 
		}
	}
	
	void display () 
	{
		Node *p = link;
		while (p != NULL)
		{
			cout<<p->m<< "->";
			p = p->link;
		}
		cout<<endl;
	}
	
	
	//원하는 값 위치 번호 찾기. 
	int find (int value)
	{
		Node *remove = link;
		int index = 0;
		while (remove != NULL)
		{
			if(remove->m == value)
			{
				cout<<"Index Number of value (" <<value<<") is = "<<index<<endl;
				//remove = remove->link;
				break;
			}
			remove = remove->link;
			index++;
		}
		
		return index;
	}
};


int main ()
{
	Node head;
	head.insert(new Node(10));
	head.link->insert(new Node(9));  //insert format1 
	head.insert(new Node(5));        //insert format2
	head.insert(new Node(24));
	
		//현재 HEAD는 24에 위치하고 있으며 24->5->9->10의 순서로 탐색하게된다. 
	
	//head.pop();
	head.display();
	head.find(10);
	return 0;
}
