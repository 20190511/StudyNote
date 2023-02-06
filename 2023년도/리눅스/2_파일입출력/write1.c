/* 키보드로 입력받고 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	int length;
	
	// 버퍼만큼 stdin(키보드) 로 부터 입력받아 buf에 저장
	// stdout(모니터) 로 출력.
	length = read(0, buf, BUFFER_SIZE);
	write(1, buf, length);
	exit(0);
}
