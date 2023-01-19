#include <iostream>
#include <string>
using namespace std;

/*
	이중 연결리스트.
		삽입 : O(1)
		원하는 위치 탐색,삽입,변경 : O(n)
		탐색 : O(n)
	
	단일 연결리스트는 Head(현재 가리키는 위치)에서 단방향으로만 이동할 수 있으므로
		원하는 값을 탐색할 순 있지만 그 값을 변경하거나 삭제하는 것이 쉽지 않음.
		
		
	이중 연결리스트 원리 (head, tail위치만을 사용해 link로 이동하는 자료구조) 
		a) tail과 head의 값은 안다.
			(tail은 prev를 사용하여 탐색할 수 있다.)
			(head는 next를 사용하여 탐색할 수 잇다.)
		
		b) 일반적으로 head에 새롭게 생성되는 newNode는
			NULL <- (prev)newNode(next) <=> (prev)head(next) -> NULL 
*/


/*Node 클래스: 자기자신(Node)의 Data값과 그 노드를 기준으로 이전,이후  노드의 주솟값 저장*/
class Node {
public:
	int data;
	Node *prev , *next;
	
	Node()
	{
		this->data = 0;
		this->prev = this->next = NULL; //초기화
	}
};



class DoubleLinkedList {
private:
	int size;               //현재 몇개의 노드를 가지고 있는지 세는 역할. 
	Node *head , *tail;     //head와 tail 노드 생성.
	
public:
	DoubleLinkedList()
	{
		this->size = 0;
		this->head = this->tail = NULL;  //현재 자기 자신의 Head값과 Tail값을 NULL값으로 만들어줌. 
	}
	
	//head에 값 추가. 
	void addNodeToHead(int value) 
	{
		
		Node *newNode = new Node();
		newNode->data = value;
		
		//비어 있는 경우에는 Head와 tail모두 newNode를 가리킴. 
		if(isEmpty())
		{
			head = tail = newNode;
		}
		else // [이해] NULL<-(p)[newNode](n) <=> (p)[head](n)->NULL 
		{
			newNode->next = head;    // newNode의 이전 주솟값 = head 
			head->prev = newNode;    // head의 다음 주솟값 = newNode. 
			head = newNode;          //head를 재설정. (가리키는 위치) 
		}
		
		this->size++;   //현재의 size값 증가. 
	}
	
	//tail에 새로운 값 추가. 
	void addNodeToTail (int value)
	{
		Node *newNode = new Node();
		newNode->data = value;
		
		if(this->size == 0) //size == 0이란 아무런 노드가 없음을 의미함.
		{
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
	
	//원하는 위치의 노드의 데이터를 원하는 값으로 변경.
	void changeNodeByLocation (int changePosition, int changeValue)
	{
		if(isEmpty() || changePosition > this->size || changePosition < 1)
		{
			return;
		}
			
			//Head, tail 위치가 중요함 (리스트구조에서는) 
		Node *originHeadNodePosition = head;  //원래 head위치를 기록하는 역할. 
		
		for(int i = 0 ; i < changePosition - 1 ; i++)
		{
			head = head->next; //head의 정방향으로 계속 이동. 
		}
		
		// 이동된 head가 가리키는 자료구조의 값을 변경 
		head->data = changeValue;
		head = originHeadNodePosition; //기억해둔 head값으로 다시 저장. 
	}
	
	
	//지정된 위치의 노드를 삭제하는 경우.
	void deleteNodeByLocation (int deletePosition)
	{
		if(isEmpty() || deletePosition < this->size || deletePosition < 1)
		{
			return;
		}
		
		Node *tempDeleteNodePosition;    //지울 노드의 위치를 임시로 저장할 노드. (Node 반환을 목적) 
		Node *originHeadPosition = head; //Head위치 기록하는 역할.
		
		
		// 위치만큼 헤드를 이동시키기. 
		for (int i = 0 ; i < deletePosition - 1 ; i++)
		{
			head = head->next; 
		}
		
		
		//지우는 케이스 (가장 처음 위치의 노드 )
		if (deletePosition == 1)
		{
			//노드가 2개 이상인 경우 
			if(this->size > 1)
			{ 
				tempDeleteNodePosition = head; //임시로 만든 노드를 head로 가리키고 
				head = head->next;             //head의 위치를 다음 head로 옮겨줍니다.
				//head->prev = NULL;           //안해줘도됨. 
				delete(tempDeleteNodePosition); //(중요) 필요없어졌으니 삭제. 
			}
			else
			{
				tempDeleteNodePosition = head;
				head = tail = NULL; //(노드가 하나밖에 없었고 이것을 삭제했으므로 모두NULL로 만들어줌) 
				delete tempDeleteNodePosition;
			}
			
			this->size--;
		}
		//지우는 케이스2  (마지막에 있는 경우) 
		else if (deletePosition == this->size) 
		{
			tempDeleteNodePosition = head; //임시 장소에 head를 둠. 
			head = head->prev;             //head의 이전 주소로 head를 이동시켜줌. 
			//head->next = NULL            //안해줘도 됨 
			tail = head;                   //그 위치를 tail로 둠.
			
			
			delete(tempDeleteNodePosition);
			
			head = originHeadPosition; //head를 원래위치로
			this->size--; 
		 } 
		 //지우는 케이스3 (중간에 그냥 끼여있는 경우) 
		 else
		 {
		 	head->prev->next = head->next;    //head를 prev로 이동시킨 위치의 next = head의 prev값 
		 	head->next->prev = head->prev;    //head를 next로 이동시킨 위치의 prev = head의 next값 
		 	
		 	tempDeleteNodePosition = head;    //임시로 지울 Node
			delete tempDeleteNodePosition;
			
			head = originHeadPosition;     //head를 원래 위치로 이동시키기. 
			this->size--; 
		 	
		 }
	}
	
	
//	//값이 존재하는지 확인해주는 함수 (head를 원래위치로 이동시키는 방식) 
	bool isSearchNodeByValue (int searchValue)
	{
		if(isEmpty())
		{
			return false;
		}
		
		Node *originHeadNodePosition = head;
		
		for(int i = 0 ; i < this->size ; i++)
		{
			if(head->data == searchValue) //존재하는 경우. 
			{
				head = originHeadNodePosition; //Head를 원래위치로 이동. 
				return true;
			}
			head = head->next; //head를 계속 이동 
		} 
		return false;    //for문을 다 돌고빠져나왔음 == 없음. 
	}
	
	
	//해당 위치의 값을 return
	int searchNodeByLocation (int searchLocation)
	{
		if (isEmpty() || searchLocation > this->size || searchLocation < 1)
		{
			return false;
		}
		
		Node *originHeadNodePosition = head;
		
		for (int i = 0 ; i < searchLocation - 1 ; i++)
		{
			head = head->next; //해당 location까지 head 이동. 
		} 
		
		int tempResultData = head->data;  //해당 searchLocation의 데이터 값 
		head = originHeadNodePosition;    //헤드 원위치 시켜줌 
		 
		return tempResultData;
	}
	
	
	//head 에서 정방향 출력
	void print()
	{
		if (isEmpty())
			return;
			
		Node *originHeadNodePosition = head;
		
		for(int i = 0 ; i < size + 1 ; i++)
		{
			cout<<head->data<<"->";
			if(!head->next) break;  // 탐색이 끝나면 "->" 안 나오게 하기위함.
			head = head->next;      //헤드 next 방향으로 계속 이동.
		}
		
		head = originHeadNodePosition; //헤드 원위치로 이동
		cout<<endl;
	 } 
	 
	 
	 //tail에서 반대방향으로 print
	 void printReverse()
	 {
	 	if (isEmpty())
	 		return;
	 		
	 	Node *originTailNodePosition = tail;
	 	
	 	for(int i = size ; i >= 0 ; i--)
	 	{
	 		cout<<tail->data<<"->";
	 		if(!tail->prev) break; //tail로 탐색을 마친 경우 
	 		tail = tail->prev;     //tail의 prev값을 재설정 
		 }
		 
		tail = originTailNodePosition;
		cout<<endl;
	 }
	 
	 
	 //isEmpty 함수 (비어있는지 확인)
	 bool isEmpty()
	 {
	 	if (this->size == 0)
	 		return true;
	 		
	 	return false;
	 } 
	 
	 
	 
	 //리스트 모두 삭제 
	 void clearList ()
	 {
	 	Node *deleteNode;
	 	for(int i = 0 ; i < this->size-1 ; i++)
	 	{
	 		deleteNode = head;
	 		if (head->next == NULL) break;
	 		
	 		head = head->next;    // 노드 next방향으로 이동. 
	 		delete deleteNode;    // 노드 완전 삭제 
		}
		
		this->size = 0;
		head = tail = NULL; 
	 }
};






int main()
{
    // test
    DoubleLinkedList testList;
 
    testList.addNodeToHead(10);
    testList.addNodeToTail(20);
    testList.addNodeToHead(30);
 
    testList.print();
    testList.printReverse();
 
    testList.changeNodeByLocation(2,200);
    testList.deleteNodeByLocation(1);
 
    testList.print();
    cout << testList.isEmpty() << endl;
 
    cout << testList.searchNodeByLocation(2) << endl;
    cout << testList.isSearchNodeByValue(200) << endl;
    cout << testList.isSearchNodeByValue(201) << endl;
 
    testList.clearList();
 
    testList.print();
    cout << testList.isEmpty() << endl;
 
    testList.addNodeToHead(1);
    testList.addNodeToTail(2);
    testList.addNodeToHead(3);
 
    testList.print();
}
