#include <stdio.h>

/*question1 전용 함수들 */
int small(int one_line[7])
{
	int value = one_line[0];
	for (int i = 1; i < 7; i++)
	{
		if (value > one_line[i])
			value = one_line[i];
	}
	return value;
}
void reset(int one_line[7])
{
	for (int i = 0; i < 7; i++)
		one_line[i] = 0;
}
void copy_s(int one_line[7], int d[][7], int matrix[][7], int i, int j)
{
	for (int x = 0; x < 7; x++)
		one_line[x] = d[i][x] + matrix[x][j];
}
void print_matrix(int matrix[][7], int m)
{
	printf("m = %d\n", m);
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

/*quetion 2,3 전용 함수*/
void copy_array(int matrix[][7], int d[][7])
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			d[i][j] = matrix[i][j];
		}
}
void print_array(int matrix[][7])
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}
/*question3 전용 함수*/
void reverse_array(int matrix[][7], int reverse_matrix[][7])
{
	int i2 = 6;
	int j2 = 6;
	for (int i = 0; i < 7; i++)
	{
		j2 = 6;
		for (int j = 0; j < 7; j++)
		{
			reverse_matrix[i][j] = matrix[i2][j2];
			j2--;
		}
		i2--;
	}
}
/*question1 전용 모든쌍 최단거리 그래프*/
void graph(int matrix[][7])
{
	int one_line[7];
	reset(one_line);
	int d[7][7];
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			d[i][j] = matrix[i][j];

	for (int m = 1; m < 6; m++)
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				copy_s(one_line, d, matrix, i, j);
				d[i][j] = small(one_line);
				reset(one_line);
			}
		}
		print_matrix(d, m + 1);
	}
}

/*question 2,3 전용 : 플로이드 그래프*/
void floyd(int matrix[][7], int d2[][7], int path[][7])
{
	copy_array(matrix, d2);

	for (int k = 0; k < 7; k++)
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				int value = d2[i][j];
				if (value > d2[i][k] + d2[k][j])
				{
					value = d2[i][k] + d2[k][j];
					path[i][j] = k + 1;             //경로에 값 추가. 
				}
				d2[i][j] = value;
			}
		}
		printf("{ k = %d Matrix }\n", k);
		print_array(d2);
		printf("\n");
	}
}


void path_cal(int path[][7], int q, int r, char node)
{
	if (path[q][r] != 0)
	{
		path_cal(path, q, path[q][r] - 1, node);
		printf("%c%d->", node, path[q][r]);
		path_cal(path, path[q][r] - 1, r, node);
	}
}

void print_path(int path[][7], int start, int end, char node)
{
	printf("%c%d->", node, start);
	path_cal(path, start - 1, end - 1, node);
	printf("%c%d\n", node, end);
}


void execution_question1(int matrix[][7])
{
	printf("\n +++++++++ Question1 : Start +++++++++\n");
	int matrix_graph[7][7] = { 0 , };
	copy_array(matrix, matrix_graph);
	printf("--- 1. [print all Graph] ---\n");
	graph(matrix_graph);
	printf("\n +++++++++ Question1 : End +++++++++\n");
}
void execution_question2(int matrix[][7], int start, int end)
{
	printf("\n +++++++++ Question2 : Start +++++++++\n");
	printf("--- 1. [print Array Process] ---\n");
	int d1[7][7] = { 0, };
	int path[7][7] = { 0, };
	floyd(matrix, d1, path);
	//	printf("---debug---\n");
	//	print_array(path);

	printf("--- 2. [print path] ---\n");
	char q1 = 'v';
	print_path(path, start, end, q1);
	printf("\n +++++++++ Question2 : End +++++++++\n");
}


void execution_question3(int matrix[][7], int start, int end)
{
	printf("\n +++++++++ Question3 (reverse) : Start +++++++++\n");
	int reverse_matrix[7][7] = { 0, };
	reverse_array(matrix, reverse_matrix);
	int d2[7][7] = { 0, };
	int path2[7][7] = { 0, };
	printf("--- 1. [print Array Process] ---\n");
	floyd(reverse_matrix, d2, path2);

	printf("--- 2. [print path] ---\n");
	char q2 = 'a';
	print_path(path2, start, end, q2);
	printf("\n +++++++++ Question3 : End +++++++++\n");
}


int main()
{
	int matrix[7][7] = { 0,4,0,0,0,10,0,3,0,999,18,999,999,999,999,6,0,999,999,999,999,999,5,15,0,2,19,5,999,999,12,1,0,999,999,999,999,999,999,999,0,10,999,999,999,8,999,999,0 };
	int reverse_matrix[7][7] = { 0, };
	reverse_array(matrix, reverse_matrix);
	//print_array(reverse_matrix);

	int d2[7][7] = { 0, };
	int path[7][7] = { 0, };

	execution_question1(matrix);
	printf("\n\n\n\n\n\n\n\n\n");
	execution_question2(matrix, 3, 6);
	printf("\n\n\n\n\n\n\n\n\n");
	execution_question3(matrix, 5, 2);


	return 1;
}
