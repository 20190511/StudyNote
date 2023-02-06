/*
실행파일 <텍스트파일명> 

입력받는 텍스트 줄의 문자를 출력해줌.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define TALBE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	//한 줄 당.
	static struct 
	{
		long offset;	// 해당 줄의 시작 offset
		int length;		// 해당 줄의 문자길이
	} table [TALBE_SIZE];

	char buf[BUFFER_SIZE];
	long offset;
	int entry;	// 줄 개수 기록
	int i;
	int length;	// offset 기록
	int fd;

	printf("%s", argv[1]);
	if (argc < 2)
	{
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "open error : %s\n", argv[1]);
		exit(1);
	}

	entry = 0;
	offset = 0;

	//우선 1024 바이트만큼 긁어온 뒤에 -> 개행문자까지 offset을 계속 기록해서 table[entry]에 기록하는 방식
	//여기서 entry는 line(줄) 번호를 의미함. (0번부터시작)
	// 1024 바이트를 긁어오고 남아있으면 read 가 0 이 될 때까지 긁어옴 (-1 이면 그냥 err)
	while ((length = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		for (int i = 0; i < length; i++)
		{
			table[entry].length++;
			offset++;

			// char값은 ' ' 으로 설정한다 " " 으로 넣지말자, 큰일난다!
			if (buf[i] == '\n')
			{
				table[++entry].offset = offset;
				printf("%d line, offset:%ld, length:%d\n", entry, offset, length);
			}
		}
	}
#define DEBUG
#ifdef DEBUG
	for (i = 0; i < entry; i++)
		printf("%d: %ld, %d\n", i + 1, table[i].offset, table[i].length);
#endif
	
	while (1)
	{
		printf("Enter line number : ");
		scanf("%d", &length);
		
		length--;
		//Segment Dump 막기위함 (메모리접근 방어.
		if (length < 0 | length >= entry)
			break;

		lseek(fd, table[length].offset, SEEK_SET);

		if (read(fd, buf, table[length].length) <= 0)
			continue;

		buf[table[length].length] = '\0';
		printf("%s", buf);
	}
	close(fd);
	exit(0);
}

