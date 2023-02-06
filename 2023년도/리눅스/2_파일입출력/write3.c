#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define NAME_SIZE	64

typedef struct {
	char name[NAME_SIZE];
	int pid;
	int salary;

}ssu_employee;


int main(int argc, char* argv[])
{
	ssu_employee record;
	int fd;

	if (argc < 2)
	{
		fprintf(stderr, "usage : %s <filename>\n", argv[0]);
		exit(1);
	}

	//O_EXCL  : 이미 존재하면 Error처리.
	if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0640)) < 0)
	{
		fprintf(stderr, "%d, file Error : %s\n", fd, argv[1]);
		exit(1);
	}

	while (1)
	{
		printf("Enter employee name <space> salary : ");
		scanf("%s", record.name);

		if (record.name[0] == '.')
			break;
		
		scanf("%d", &record.salary);
		//getpid() :-> 프로세스 id를 받아오는 함수 (현재 프로세스 pid를 가져와서 저장).
		record.pid = getpid();
		printf("pid = %d\n", record.pid);

		// &record 구조체 포인터 위치에서 sizeof(record) 만큼 write 하겠다는 의미이다.
		// (char*)&record 해주면 casting되어 더 자세한 값을 얻을 수 있다.
		write(fd, &record, sizeof(record));
		printf("sizeof(char(*)struct) = %ld, %s\n",sizeof((char*)&record),(char*)&record);
		
	}
	close(fd);
	exit(0);
}
