/* 실행파일 <파일1> <파일2>
	파일1 -> 파일2로 복사*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define CFLAGS (O_WRONLY | O_CREAT | O_TRUNC)


int main(int argc, char* argv[])
{
	char buf[BUFFER_SIZE];
	int fd1, fd2;
	int length;

	if (argc != 3)
	{
		fprintf(stderr, "Arguement Error !");
		exit(1);
	}

	/* fd1,fd2 복사 */
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "File Error : %s", argv[1]);
		exit(1);
	}
	if ((fd2 = open(argv[2], CFLAGS, 0644)) < 0)
	{
		fprintf(stderr, "File Error : %s", argv[1]);
		exit(1);
	}

	/* fd1 read 후 fd2 복사*/
	while ((length = read(fd1, buf, BUFFER_SIZE)) > 0)
	{
		if (write(fd2, buf, length) != length)
		{
			buf[length] = '\0';
			fprintf("COPY Error : %s", buf);
			exit(1);
		}
	}
	exit(0);
}
