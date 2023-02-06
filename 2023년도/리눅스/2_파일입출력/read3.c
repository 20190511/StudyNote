#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define NAME_SIZE 64

typedef struct {
	char name [NAME_SIZE];
	int pid;
	int salary;
}ssu_employee;


int main(int argc, char* argv[])
{
	ssu_employee record;
	int fd;
	int record_num;

	if (argc<2)
	{
		fprintf(stderr, "Usage : %s file\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "OPEN %s\n",argv[1]);
		exit(1);
	}

	while (1)
	{
		printf("Enter Record number : ");
		scanf("%d", &record_num);
		
		if (record_num < 0)
			break;

		if (lseek(fd,(long)record_num*sizeof(record),SEEK_SET) < 0)
		{
			fprintf(stderr, "lseek Error\n");
			exit(1);	
		}

		if (read(fd, (char*)&record, sizeof(record)) > 0)
		{
			printf("Empolyee : %s Salary : %d\n",record.name, record.salary);
		}
		else
			printf("Record %d is not found\n", record_num);
	}
	close(fd);
	exit(0);
}
