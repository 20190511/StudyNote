#include <iostream>
#include "QueueElement.h"
using namespace std;





int main(void)
{
	List newList;
	newList.insert(10);
	newList.insert(20);
	newList.insert(30);

	newList.print();


	newList.pop();
	newList.print();
	return 1;
}
