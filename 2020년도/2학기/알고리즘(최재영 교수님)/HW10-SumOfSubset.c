#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//0. 최종적인 SUM_VALUE 값. 
static int SUM_VALUE = 52;

//1. 부분합 종류를 출력해주는 함수 
void print_matrix_index(int index_array[], int array[])
{
	printf(" matrix subset = {");
	for (int i = 0; i < 6; i++)
	{
		if (index_array[i] == 0)
			printf("%d,", array[i]);
	}
	printf("}\n");
}

//2. sum of Subset 의 promising 함수 (조건함수) 
int promise(int array[], int weight, int total, int index)
{
	//바닥조건 && (최종 목적 || 최대 조건) 
	if ((weight + total >= SUM_VALUE) && (weight == SUM_VALUE || weight + array[index] <= SUM_VALUE))
		return 1;
	else
		return 0;
}



/* [Main 1] : 최종 Sum값에서 array[index]를 빼가며 subset을 구하는 방식 (역방식)*/
// index = 가장 최고인덱스 - 1 
void subsetOfSum(int array[], int index_array[], int index, int sum)
{
	//교수님 께서 대략 4~5개 프로세스 과정을 출력하라고 지시하셨음.
	if (index >= 4)
	{
		printf("현재 검사 중인 합집합 원소들\n");
		print_matrix_index(index_array, array);
	}
		
	if (sum == 0)
	{
		printf("\n---------------------------------------------\n");
		printf("합이 %d가 되는 합집합 출력\n", SUM_VALUE);
		print_matrix_index(index_array, array);
		printf("\n");
		printf("---------------------------------------------\n");
	}
	//이 조건이 promising이 됨. 
	else if (sum < 0 || index == -1)
		return;
	else
	{
		//해당하는 인덱스 값을 빼고 넘어가는 경우 
		index_array[index] = 0;
		subsetOfSum(array, index_array, index - 1, sum - array[index]);
		//해당하는 인덱스 값을 빼지 않고 넘어가는 경우 
		index_array[index] = 1;
		subsetOfSum(array, index_array, index - 1, sum);
	}
}

int sum_array(int array[])
{
	int sum = 0;
	for (int i = 0; i < 6; i++)
		sum += array[i];
	return sum;
}



/* [Main 2] : 강의 피피티와 동일한 상태함수 방식*/
void subsetOfSum2(int array[], int index_array[], int index, int weight, int total)
{
	int promise = 1000;
	if ((weight + total >= SUM_VALUE) && (weight == SUM_VALUE || weight + array[index] <= SUM_VALUE))
		promise = 1;
	else
		promise = 0;

	//교수님 께서 대략 4~5개 프로세스 과정을 출력하라고 지시하셨음.
	if (index < 3)
	{
		printf("현재 검사 중인 합집합 원소들\n");
		print_matrix_index(index_array, array);
	}


	if (promise)
	{
		if (weight == SUM_VALUE)
		{
			printf("\n---------------------------------------------\n");
			printf("합이 %d가 되는 합집합 출력\n", SUM_VALUE);
			print_matrix_index(index_array, array);
			printf("\n");
			printf("---------------------------------------------\n");
		}
			
		else
		{
			index++;
			//위와 반대로 추가될 때마다, 0으로 변경
			index_array[index] = 0;
			subsetOfSum2(array, index_array, index, weight + array[index], total - array[index]);

			//추가되지 않으면 1로 변경
			index_array[index] = 1;
			subsetOfSum2(array, index_array, index, weight, total - array[index]);
		}
	}
}




//무조건 모두 순회는 해야함. (조건범위 내에서) 
//자식은 무조건 2개 있으므로 2개 중 하나 랜덤으로 뽑기 -> rand로 뽑牡.
	/*자식고르기: 1) array[index] or 2) 0 중 하나를 더한 값. */

//원래라면 -> index 값을 마구잡이로 넣을 수 있었으나, 문제에서 위의 backtracking 알고리즘이라고 하였으므로
//{ 2,10,13,17,22,42 } 와SUM_VALUE 는 52로 그대로 두고 하겠습니다. 
int estimate_subsetOfSum(int array[], int index)
{
	int m = 1;
	int numNode = 1;
	int mprod = 1;
	int i = 0;
	int weight = 0;
	int total = sum_array(array);

	//weight == 52가 되는 경우 promising 하더라도 알고리즘이 진행하지 않음. 
	while (m != 0 && i != index && weight != SUM_VALUE)
	{
		int t = 2; //자식의 수 
		mprod *= m;
		numNode += mprod * t;

		//현재 노드의 promising 하는 노드의 수 찾기. -> promising 하면 2개 아니면 0개 
		m = 0;
		if (promise(array, weight, total, i))
			m = 2;

		//다음 자식에 선택할 노드 ->  랜덤값으로 위의 방식으로 자식 차출 
		if (m != 0)
		{
			//난수가 홀수면 array[i]값을 추가함, 짝수면 추가하지 않겠음. 
			int random_value = rand();
			if (random_value % 2 == 1)
			{
				weight += array[i];
			}
			else
			{
				weight += 0;
			}
			total -= array[i];
			i++;
		}
	}
	return  numNode;
}


/* Execution 전용 함수들*/
// 가 . 부분 합 집합 구하기 1번 함수. (SUM_VALUE에 해당 array[index]를 빼가며 sum == 0인지 확인하는 알고리즘 방식 
void execution_function1(int array[], int index_array[])
{
	printf("\n +++++++++ 1. subsetOfSum func 1 : Start +++++++++\n");
	//version 1
	subsetOfSum(array, index_array, 5, SUM_VALUE);
	printf(" +++++++++ 1. subsetOfSum func 1 : END +++++++++\n");
}

// 나. 부분 합 집합 구하기 2번 함수 . (강의에서 소개하는 상태함수 방식 subsetOfSum 알고리즘. 
void execution_function2(int array[], int index_array2[])
{
	printf("\n +++++++++ 2. subsetOfSum func2 : Start +++++++++\n");
	//version 2
	int sum = sum_array(array);
	subsetOfSum2(array, index_array2, 0, 0, sum);
	printf(" +++++++++ 2. subsetOfSum func2 : END +++++++++\n");

}

//다. Monte Carlo 알고리즘으로 추정치 계산함수
void execution_function3(int array[])
{
	int sum = 0;
	printf("\n +++++++++ 3. Monte Carlo [Sum of Subsets] : Start +++++++++\n");
	printf("20번 돌려서 나온 값의 평균을 추정치로 계산합니다.\n");
	for (int i = 0; i < 20; i++)
	{
		sum += estimate_subsetOfSum(array, 5);
	}
	sum = sum / 20;
	printf(" Monte Carlo estimate value = %d", sum);
	printf("\n +++++++++ 3. Monte Carlo [Sum of Subsets] : END +++++++++\n\n");
}



int main(void)
{
	//S = 52
	int array[6] = { 2,10,13,17,22,42 };
	int index_array[6] = { 1,1,1,1,1,1 };
	int index_array2[6] = { 1,1,1,1,1,1 };

	execution_function1(array, index_array);
	execution_function2(array, index_array2);
	execution_function3(array);

	return 1;
}
