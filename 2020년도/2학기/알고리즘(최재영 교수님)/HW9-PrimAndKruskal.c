#include <stdio.h>

/*사이클 형성은 합집합 판단 알고리즘으로 구현하였음*/

// 가장 위의 조상의 부모 찾기
int parentFind(int set[], int node)
{
	if (set[node] == node)
		return node;
	return set[node] = parentFind(set, set[node]);
}

//해당 노드의 가장 위의 조상 (가장 작은 노드)에 노드 넣기. 
int unionParent(int set[], int nodeA, int nodeB)
{
	nodeA = parentFind(set, nodeA);
	nodeB = parentFind(set, nodeB);
	if (nodeA < nodeB)
		set[nodeB] = nodeA;
	else
		set[nodeA] = nodeB;
}

//사이클을 형성하면=1 , 형성하지 않으면 = 0 
int findParent(int set[], int nodeA, int nodeB)
{
	nodeA = parentFind(set, nodeA);
	nodeB = parentFind(set, nodeB);
	if (nodeA == nodeB)
		return 1;
	else
		return 0;
}

//cycle set 함수 설정 함수
void cycle_set(int set[])
{
	for (int i = 0; i < 8; i++)
		set[i] = i;
}



/*kruskal , prim 모두 사용하는 함수들*/
//8x8 배열 출력함수
void print_array(int array[][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			printf("%d ", array[i][j]);
		printf("\n");
	}
}

//초기 prim,kruskal 행렬 설정 함수(모두 999로 리셋되어있음)
void graph_set(int prim[][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			prim[i][j] = 999;
}

//Prim ,kruskal알고리즘 경로 출력
void print_path(int prim[][8])
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = row; col < 8; col++)
		{
			if (prim[row][col] != 999)
			{
				printf("V%d -> V%d , length of path = %d.\n", row + 1, col + 1, prim[row][col]);
			}
		}
	}
}


/*Prim 전용 함수들*/
// 1.Prim 알고리즘에서 minimum 배열 999로 리셋해주는 함수. (매 for문 마다 최솟값 구할 때 사용)
void minimum_value_set(int array[])
{
	for (int i = 0; i < 8; i++)
		array[i] = 999;
}

// 2. 해당 행에서 가장 작은 값 구하기.
int minimum_row(int origin[][8], int row, int next_small)
{
	int min_value = origin[row][row];
	for (int i = 0; i < 8; i++)
		if (origin[row][i] < min_value && origin[row][i] > next_small)
			min_value = origin[row][i];
	return min_value;
}

// 3. small_value 보다는 크면서 다음으로 작은 값 리턴.
int find_minIndex(int origin[][8], int row, int small_value)
{
	for (int i = 0; i < 8; i++)
	{
		if (origin[row][i] == small_value)
			return i;
	}
	return NULL;
}

// 4. check2 (앞의 합집합 알고리즘이 다 해결해주긴 하지만, 비상용 알고리즘)
int queue_check(int queue[], int value)
{
	for (int i = 0; i < 8; i++)
	{
		if (queue[i] == value)
			return 1;
	}
	return 0;
}

// 5. while문을 디버깅하는데 시간이 많이 걸려서 제작
int debug_prim_while(int origin[][8], int prim[][8], int minimum_value[], int prim_queue[], int cycle_judge[], int start, int queue_count)
{
	int minimum = 999;
	while (1)
	{
		minimum = 999;
		int minRow = -1;
		for (int k = 0; k < start + 1; k++)
		{
			if (minimum > minimum_value[k])
			{
				minimum = minimum_value[k];
				minRow = k; //해당하는 행 값 찾음 (핵심) 
			}
		}
		int minIdx = find_minIndex(origin, prim_queue[minRow], minimum); //해당하는 가장 작은 값 열값 찾기 
		int check_prim_cycle = findParent(cycle_judge, prim_queue[minRow], minIdx);
		int check_queue_2 = queue_check(prim_queue, minIdx);
		if (check_prim_cycle == 0 && check_queue_2 == 0)
		{
			unionParent(cycle_judge, minRow, minIdx);
			prim_queue[queue_count++] = minIdx;
			prim[prim_queue[minRow]][minIdx] = minimum;
			prim[minIdx][prim_queue[minRow]] = minimum;
			break;
		}
		else
		{
			minimum_value[minRow] = minimum_row(origin, prim_queue[minRow], minimum);
		}
	}
	return minimum;
}



// [Main] Prim 알고리즘 함수
void prim_algorithm(int origin[][8], int prim[][8])
{
	//합집합 알고리즘 활용 배열
	int cycle_judge[8] = { 0, };
	cycle_set(cycle_judge);

	graph_set(prim);
	int prim_queue[8] = { 0, };
	int queue_count = 0;

	//초기 값 
	prim_queue[queue_count++] = 0;

	for (int start = 0; start < 7; start++)
	{
		int minimum_value[8] = { 0, };
		minimum_value_set(minimum_value);

		// 0 < start < 8
		for (int k = 0; k < start + 1; k++)
		{
			minimum_value[k] = minimum_row(origin, prim_queue[k], -1);
		}
		debug_prim_while(origin, prim, minimum_value, prim_queue, cycle_judge, start, queue_count);
		queue_count++;
	}
}

//



/* kruskal Alogorithm 함수 */

// 0. 함수 출력함수 (num값에 따라 달라지는 배열의 경우) -> 디버깅 전용
void print_array_num(int array[], int num)
{
	for (int i = 0; i < num; i++)
		printf("%d ", array[i]);
	printf("\n");
}



// 1. 배열 스왑 함수 
void swap_array(int arr1[], int i, int arr2[], int j)
{
	int temp = arr1[i];
	arr1[i] = arr2[j];
	arr2[j] = temp;
}

// 2. kruskal 함수 설정
void kruscal_edge_set(int kruskal[], int num)
{
	for (int i = 0; i < num; i++)
		kruskal[i] = 999;
}

//3_1. quickSort 호출 함수
int partition(int copy_array[], int row[], int col[], int left, int right)
{
	int pivot = copy_array[left];
	int low = left + 1;
	int high = right;

	while (low <= high)
	{
		while (copy_array[low] <= pivot && low <= right)
			low++;
		while (copy_array[high] >= pivot && high >= (left + 1))
			high--;
		if (low <= high)
		{
			//swap(copy_array[low], copy_array[high]);
			int temp = copy_array[low];
			copy_array[low] = copy_array[high];
			copy_array[high] = temp;
			swap_array(row, low, row, high);
			swap_array(col, low, col, high);
		}

	}
	//swap(copy_array[left], copy_array[high]);
	int temp = copy_array[left];
	copy_array[left] = copy_array[high];
	copy_array[high] = temp;
	swap_array(row, left, row, high);
	swap_array(col, left, col, high);
	return high;
}

//3_2. quick Sort 정렬 함수
void quick_sort(int copy_array[], int row[], int col[], int left, int right)
{
	if (left <= right)
	{
		int pivot = partition(copy_array, row, col, left, right);
		quick_sort(copy_array, row, col, left, pivot - 1);
		quick_sort(copy_array, row, col, pivot + 1, right);
	}
}


// 4. node 정렬 함수 ( kruskal 값들 정렬, 행정렬 , 열정렬) 
void node_sort(int origin[][8], int kruskal[][8], int kruskal_row[], int kruskal_col[], int kruskal_value[])
{
	int count = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 8; j++)
		{
			if (origin[i][j] != 999)
			{
				kruskal_value[count] = origin[i][j];
				kruskal_row[count] = i;
				kruskal_col[count] = j;
				count++;
			}
		}
	//quick sort 방식으로 정렬 
	quick_sort(kruskal_value, kruskal_row, kruskal_col, 0, 13);
	//	print_array(origin);

	//	print_array_num (kruskal_value , 14);
	//	print_array_num (kruskal_row,14);
	//	print_array_num (kruskal_col,14);
}


// [Main] kruskal Algorithm
int kruskal_algorithm(int origin[][8], int kruskal[][8], int kruskal_row[], int kruskal_col[], int kruskal_value[])
{
	int sum = 0; //8개 노드의 간선의 가중치 총합 계산
	//합집합 알고리즘 배열
	graph_set(kruskal);
	int cycle_judge_2[8] = { 0, };
	cycle_set(cycle_judge_2);
	node_sort(origin, kruskal, kruskal_row, kruskal_col, kruskal_value);

	int count = 0;
	int index = 0;
	while (1)
	{
		// 간선 개수 = 노드 개수 -1 = 14 - 1= 13개이므로 14개째일 때 break 
		if (count == 7)
			break;
		int row = kruskal_row[index];
		int col = kruskal_col[index];
		int val = kruskal_value[index];
		int check_kruskal = findParent(cycle_judge_2, row, col);
		if (check_kruskal == 0)
		{
			unionParent(cycle_judge_2, kruskal_row[index], kruskal_col[index]);
			kruskal[row][col] = val;
			kruskal[col][row] = val;
			sum += val;
			count++;
		}
		index++;
	}
	return sum;
}



/* Execution 전용 함수들*/
// 가 . Prim 알고리즘 출력 함수 (Prim  행렬 + Prim 경로와 가중치)
void execution_prim(int origin[][8], int prim[][8])
{
	printf("\n +++++++++ 1. Prim Graph : Start +++++++++\n\n");
	prim_algorithm(origin, prim);
	printf("--- [print Array] ---\n\n");
	print_array(prim);
	printf("--- [print Prim Path] :(Arrow) ---\n\n");
	print_path(prim);
	printf("\n +++++++++ PrimGraph : End +++++++++\n");
}

// 나. Kruskal 알고리즘 출력 함수 (Kruskal 행렬 + Kruskal 경로와 가중치)
void execution_kruskal(int origin[][8], int kruskal[][8], int kruskal_row[], int kruskal_col[], int kruskal_value[])
{
	printf("\n +++++++++ 2. Kruskal Graph : Start +++++++++\n");
	int sum = kruskal_algorithm(origin, kruskal, kruskal_row, kruskal_col, kruskal_value);
	printf("--- [print Array] ---\n\n");
	print_array(kruskal);
	printf("--- [print Kruskal Path] :(Arrow) ---\n\n");
	print_path(kruskal);
	printf("--- Kruskal 알고리즘의 가중치 총합 결과 ---\n\n");
	printf("sum = %d", sum);
	printf("\n\n +++++++++ Kruskal Graph  : End +++++++++\n");

}







int main()
{

	/* 1. Original Array Set*/
	int origin[8][8] = { 999,11,9,8,999,999,999,999,11,999,3,999,8,8,999,999,9,3,999,15,999,12,1,999,8,999,15,999,999,999,10,999,999,8,999,999,999,7,999,4,999,8,12,999,7,999,999,5,999,999,1,10,999,999,999,2,999,999,999,999,4,5,2,999
	};
	/* 2. Prim Algorithm Array Set*/
	int prim[8][8] = { 0, };

	/* 3. kruscal Algorithm Array Set*/
	int kruskal_row[14] = { 0, };
	int kruskal_col[14] = { 0, };
	int kruskal_value[14] = { 0, };
	int kruskal[8][8];

	/*Main Execution*/
	execution_prim(origin, prim);
	execution_kruskal(origin, kruskal, kruskal_row, kruskal_col, kruskal_value);
	return 0;
}
