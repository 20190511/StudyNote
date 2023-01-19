#include <iostream>
using namespace std;

template <typename T1, typename T2>
void print(T1 a, T2 b)
{
	cout<<a<<", "<<b<<endl;
}



template <typename T, int size>   //배열에서 size를 선언해줄 수도있음. 
void print2 (T* arr)
{
	for (int i = 0 ; i < size ; i++)
	{
		cout<<"["<<i<<"]"<<arr[i]<<endl<<endl;
	}
} 

template <typename T>
T aaa (T a)
{
	return a + a;
}


int main(void)
{
	print(10,0.5f);
	print<int> (1.6f, 0.5f);  //typename을 명시적으로 사용. 
	print<const char*> ("abc","def");
	/*-------------------------------------------------*/
	
	
	int arr1[5] = {1,2,3,4,5};
	float arr2[5] = {1.2f, 1.3f, 1.4f, 1.5f, 1.6f};
	 
	print2<int,5>(arr1);           //배열에서 명시적으로 size를 선언해줘야함. 
	print2<float,5>(arr2);
	/*-------------------------------------------------*/
	
	
	
	cout<< aaa<int>(2) << endl;
	cout<< aaa<double>(2.2) <<endl;
	
	
	/*-------------------------------------------------*/
	return 1;
}
