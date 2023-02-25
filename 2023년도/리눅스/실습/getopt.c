#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void help();

int main(int argc, char **argv) {
	int c;
	extern char *optarg;
	extern int optind;

	printf("Current Option Number: %d\n",optind);

	while( (c=getopt(argc, argv, ":abc:d:")) != -1) {
		switch(c) {
			case 'a':
				printf("Option : a");
				break;

			case 'b':
				printf("Option : b");
				break;

			case 'c':
				printf("Option : c, Argument : %s",optarg);
				break;

			case 'd':
				printf("Option : d, Argument : %s",optarg);

			case ':':
				if(optopt=='c') printf("ccc");
				if(optopt=='a') printf("aaa");
				if(optopt=='b') printf("bbb");
				if(optopt=='d') printf("ddd");
				break;

			case '?':
				help();
				break;
		}

		printf("\tNext Option Number : %d\n",optind);
	}

	return 0;
    
}


void help() 
{ 
    printf("Usage: ./testopt [OPTION] [FILE]" 
           "  -h                도움말" 
           "  -f [FILE]         파일출력" 
           "  -v                버전출력"); 
    exit(0); 
} 
 

/**
 * optarg : --> 옵션 뒤 파라미터 값이 저장됨 (extern char *optarg)
 *          옵션 뒤에 별도의 파라미터 값이 오는 경우, 
 *          이를 파싱한 결과 파라미터 값은 optarg에 문자열로 저장된다.
 * 
 * optind : 다음번 처리될 옵션의 인덱스이다.  
 *          만약 파싱한 옵션이후에 추가적인 파라미터를 받는다면 
 *          (예를 들어 입력 파일 이름 같이) 이 값을 활용할 수 있다. 
 *          getopt()함수는 한 번 호출될 때마다 이 값을 업데이트한다.
 * 
 * opterr : 옵션에 문제가 있을 때, 이 값은 0이 아닌 값이되며, 
 *          getopt()함수가 메시지를 표시하게 된다.
 * 
 * optopt : 알 수 없는 옵션을 만났을 때 해당 옵션이 여기에 들어간다.   
 *          (이 때 getopt의 리턴값은 ‘?’가 된다.)
 * 
 * 
 * 
 * junhyeong@DESKTOP-RBDH1KS:~/go2$ ./getopt -abc agr
 * Current Option Number: 1
 *  Option : a      Next Option Number : 1
 *  Option : b      Next Option Number : 1
 *  Option : c, Argument : agr      Next Option Number : 3
 *
 *  junhyeong@DESKTOP-RBDH1KS:~/go2$ ./getopt -abc agr -d arg
 *  Current Option Number: 1
 *  Option : a      Next Option Number : 1
 *  Option : b      Next Option Number : 1
 *  Option : c, Argument : agr      Next Option Number : 3
 *  Option : d, Argument : arg      Next Option Number : 5
*/
