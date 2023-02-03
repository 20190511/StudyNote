#include <stdio.h>
#include <string.h>
#define BUFFSIZE	256 
int main(void)
{
	FILE *fp = fopen("blank.c", "r");
	if (fp == NULL)
	{
		printf("ERROR");
	}	
	
	
	char buffer[256];
	int cnt = 0;
	while (fgets(buffer,BUFFSIZE,fp) != EOF && cnt < 500)
	{
		cnt++;
		puts(buffer);
	}
	fclose(fp);
	return 0;
}
