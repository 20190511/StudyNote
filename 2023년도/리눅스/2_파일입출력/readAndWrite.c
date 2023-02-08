#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	int rfd, wfd, n;
	char buf[10];

	rfd = open("HELLO.txt", O_RDONLY);
	if (rfd == -1)
	{
		perror("Open Hello.txt error !");
		exit(1);
	}
	wfd = open("HELLO.bak", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (wfd == -1)
	{
		perror("Open Hello.bak.");
		exit(1);
	}

	while((n = read(rfd,buf,6)) > 0)
		if (write(wfd, buf, n) != n) perror("Write");
	if (n == -1) perror("READ");
	close(rfd);
	close(wfd);
	return 0;
}
