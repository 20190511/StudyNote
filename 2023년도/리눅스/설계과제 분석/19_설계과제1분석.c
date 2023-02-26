#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUFFER_SIZE 5000 
#define STD_NUM 100
#define MAX_STR_LEN 4000
#define KILL_TIME 5
#define SECOND_TO_MICRO 1000000

void ssu_runtime(struct timeval* begin_t, struct timeval*end_t) {
    end_t -> tv_sec -= begin_t -> tv_sec;

    if(end_t -> tv_usec < begin_t -> tv_usec) {
        end_t -> tv_sec--;
        end_t -> tv_usec += SECOND_TO_MICRO;
    }
    
    end_t -> tv_usec -= begin_t -> tv_usec;
    printf("Runtime : %ld:%06ld(sec:usec)\n", end_t -> tv_sec, end_t -> tv_usec);
}


struct Std {
    char id[10];
    char path[100];
    char *answer[100];
    int card[100];
};
struct Q {
    char name[30];
    char path[30];
    float score;
    int blank_q;//빈칸문제이면 1 아니면 0
    int answer_num;
    char *answer[100];
};
typedef struct MultipleArg {
    char *tmp;
    char *filename;
    char *error;
}MultipleArg;

char *error_dir = "./err";
struct Q q[100];
struct Std std[22];
int QStart = 4;

void creat_score_table(char* ANS,int q_num);
int creat_question_table(char *ANS);
void sort(int *list, int count); 
int creat_std_table(char *STD, int q_num);
void compare(char * STD, int std_num, int q_num); 
int retToken(int i, char *inp);
void creat_score(char *ANS, int std_num, int q_num);
void execute_std(int st, int a);
void trim(char* ptr); //문자열 좌우 공백 모두 삭제
void *t_function(void* multiple_arg);
static void killer(void *arg);
static void *control(void *arg);
void DeleteAllFile(char* szDir);
void capital(char *ptr);
extern int optind;
extern char* optarg;


int main(int argc, char * argv[]){

    int fd_t, fd1, fd2;
    int q_num, std_num;
    int i, j;
    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);

    if(argc < 3) {
        fprintf(stderr, "usage : %s <file>\n", argv[0]);
        exit(1);
    }

    int flag_e =0, flag_p=0, flag_t =0, flag_h=0, flag_c=0;
    int c;
    char *std_ids[5];
    char *q_names[5];
    int c_num;
    int t_num;
    char* dir[2]; // 학생답안폴더명, 답안지

    //옵션을 걸었을 경우
    while((c = getopt(argc, argv, "e:pt:c:h")) != -1) {

        switch(c) {
            
            case 'e' : // DIRNAME/학번/문제번호_error.txt에 에러 메시지가 출력
                flag_e = 1;
                error_dir = optarg;
                mkdir(error_dir, 0777);
                break;
                
            case 'p' : //채점을 진행하면서 각 학생의 점수 출력 및 전체 평균 출력
                flag_p = 1;
                break;

            case 't' : //-t [QNAMES] 옵션 : QNAME을 문제 번호로 하는 문제는 컴파일 시 –lpthrad 옵션 추가
                flag_t = 1;
                i=0;
                q_names[i] = optarg;
                i++;
                //optind가 해당 옵션 인자 중 첫번째 값을 나타냄, 그래서 다음옵션(-)을 만나기 전까지 반복.
                for(i, optind; i<5 && optind < argc && *argv[optind] != '-'; i++, optind++)
                    q_names[i] = argv[optind];
                t_num = i;
                break;

            /*//-c [STUDENTIDS] 옵션 : 채점결과 파일이 있는 경우 해당 학생들의 점수 출력 
                                        => <STUDENTDIR> <TRUESETDIR>가 없어도 사용 가능*/
            case 'c' : 
                flag_c = 1;
                i=0;
                std_ids[i] = optarg;
                i++;
                for(i, optind; i<5 && optind<argc && *argv[optind]!= '-'; i++, optind++)
                    std_ids[i] = argv[optind];
                c_num = i;
                break;

            case 'h' : // -h 옵션 : 사용법 출력
                flag_h =1;
                break;

            case '?' : // optopt = 알 수 없는 옵션을 만났을 때 해당 옵션이 여기에 들어간다.
                if(optopt =='c'||optopt == 'e' || optopt =='t') {
                    printf("option requires value\n");
                    break;
                }
                printf("Unknown flag : %c", optopt);
                break;
        }
    }

    i=0;
    
    /** 남은 인자들
     * 을 넣어둠*/
    while(optind < argc) {
        dir[i] = argv[optind];
        i++;
        optind++;
    }

    if(flag_e) 
    {
        // DIRNAME/학번/문제번호_error.txt에 에러 메시지가 출력
        q_num = creat_question_table(dir[1]);
        creat_score_table(dir[1],q_num);
        std_num = creat_std_table(dir[0], q_num);
        printf("grading student's test papers..\n"); 
        compare(dir[0], std_num, q_num);
        creat_score(dir[1], std_num, q_num);
        gettimeofday(&end_t, NULL);
        ssu_runtime(&begin_t, &end_t); //시간 체크
    }
    if(flag_p) {
        
        int nResult, Result;
        double total = 0;
        double sum=0;
        int fd;
        int i;
        char buf[BUFFER_SIZE];
        char tmp[30];
        char temp[30];

        // 채점 기준이 될 정답이 있는 디렉토리 
        strcpy(tmp, dir[1]);
        strcat(tmp, "/score_table.csv");

        strcpy(temp, dir[1]);
        strcat(temp, "/score.csv");

        // score.csv 접근 가능하면 QStart = 6(가능) or 4(불가능)
        if(Result =access(temp, 0) == 0) {
            QStart = 6;
        }
        else if(Result == -1) {
            QStart = 4;
        }
        // score_table.csv 접근 가능하면 QStart = 6(가능) or 4(불가능)
        if((nResult = access(tmp, 0))==0){

            fd = open(tmp,O_RDONLY);

            q_num = creat_question_table(dir[1]);
            
            //sizeof는 \0 까지 세줘서 6으로 세줌 (strlen은 \0은 제외하고 5로 셈)
            lseek(fd, sizeof(",0.90"), SEEK_SET);
            
            for(i=QStart; i<q_num; i++) {
                lseek(fd, sizeof(q[i].name)+1, SEEK_CUR);
                read(fd, buf, 5);
                q[i].score = atoi(buf);
            }
            close(fd);

            std_num = creat_std_table(dir[0], q_num);
            compare(dir[0], std_num, q_num);
            creat_score(dir[1], std_num, q_num);
        }
        else if(nResult == -1) {
        
            q_num = creat_question_table(dir[1]);
            creat_score_table(dir[1], q_num);
            std_num = creat_std_table(dir[0], q_num);
            compare(dir[0], std_num, q_num);
            creat_score(dir[1], std_num, q_num);

        }
        for(j=0; j<std_num; j++) {
            sum = 0;
            for(i=0; i<q_num; i++) {
                if(std[j].card[i] == 1){
                    sum += q[i].score;
                    total += sum;
                }
            }
         }
        printf("Total average : %.1f\n", total/std_num);
gettimeofday(&end_t, NULL);
    ssu_runtime(&begin_t, &end_t);
    }

    if(flag_t) {
        int nResult, Result;
        int k;
        char tmp[30];
        double total = 0;
        double sum=0;
        strcpy(tmp, dir[1]);
        strcat(tmp, "/score_table.csv");

        char temp[30];
        strcpy(temp, dir[1]);
        strcat(temp, "/score.csv");
        if(Result=access(temp, 0) == 0) {
            QStart = 6;
        }
        else if (Result == -1){
            QStart = 4;
        }
        if((nResult = access(tmp, 0))==0){
            printf("score_table.csv is exist\n");
        }
        else if(nResult == -1) {
            q_num = creat_question_table(dir[1]);
            creat_score_table(dir[1], q_num);
            std_num = creat_std_table(dir[0], q_num);
            compare(dir[0], std_num, q_num);
            creat_score(dir[1],std_num, q_num);
         
        }
        for(j=0; j<std_num; j++) {
            sum = 0;
            for(i=0; i<q_num; i++) {
                for(k=0; k<t_num; k++) {
                    if(std[j].card[i] == 1&&q_names[k]){
                        sum += q[i].score;
                        total += sum;
                    }
                }
             }
             printf("%s is finished... score :%.1f\n",std[j].id, sum);
        }
        printf("Total average : %.1f\n", total/std_num);
gettimeofday(&end_t, NULL);
    ssu_runtime(&begin_t, &end_t);
        
    }
    if(flag_h) {printf( "Usge : ssu_score <STUDENTDIR> <TRUEDIR> [OPTION]\n"
                        "Option :\n"
                        "-e <DIRNAME> print error on 'DIRNAME/ID/qname_error.txt' file\n"
                        "-t <QNAMES> compile QNAME.c with -lpthread opton\n"
                        "-h print usage\n"
                        "-p printt student's score and total averge\n"
                        "-c <IDS> print ID's score\n");
gettimeofday(&end_t, NULL);
    ssu_runtime(&begin_t, &end_t);
    }
     if(flag_c) {

    int nResult;
    int k;
    char tmp[30];
    double total = 0;
    double sum=0;
    strcpy(tmp, dir[1]);
    strcat(tmp, "/score_table.csv");
    if((nResult = access(tmp, 0))==0){
        printf("score_table.csv is exist\n");
    }
char temp[30];
        strcpy(temp, dir[1]);
        strcat(temp, "/score.csv");
        if(access(temp, 0) == 0) {
            QStart = 4;
        }
    else if(nResult == -1) {
          q_num = creat_question_table(dir[1]);
          creat_score_table(dir[1],q_num);
          std_num = creat_std_table(dir[0], q_num);
          compare(dir[0], std_num, q_num);
          creat_score(dir[1],std_num, q_num);
          
    }

    for(k=0; k<c_num; k++) {
        sum = 0;
        for(j=0; j<std_num; j++) {
            if(strcmp(std_ids[k],std[j].id)==0){
                for(i=0; i<q_num; i++) {
                    if(std[j].card[i] == 1){
                        sum += q[i].score;
                    }
                }
                printf("%s's score : %.1f\n", std_ids[k], sum);

                break;
            }
        }
     }gettimeofday(&end_t, NULL);
    ssu_runtime(&begin_t, &end_t);
     
    }




    /*score_table이 존재*/
int nResult;
char tmp[30];
strcpy(tmp, dir[1]);
strcat(tmp, "/score_table.csv");

    if((nResult = access(tmp, 0))==0){ //존재
        printf("score_table.csv is exist\n");
    }
    else if(nResult == -1){
        char temp[30];
        strcpy(temp, dir[1]);
        strcat(temp, "/score.csv");
        if(access(temp, 0) == 0) { //존재
            QStart = 4;
        }
        q_num = creat_question_table(dir[1]);

        creat_score_table(dir[1],q_num);
        mkdir(error_dir, 0777);
        std_num = creat_std_table(dir[0], q_num);
        printf("grading student's test papers..\n");
        compare(dir[0], std_num, q_num);
        creat_score(dir[1], std_num, q_num); 
        
    }
gettimeofday(&end_t, NULL);
    ssu_runtime(&begin_t, &end_t);
}




int creat_question_table(char *ANS) {
    
   char buf[BUFFER_SIZE]; 
   int fsize;
   int k;
    char folderlist[50];
    int list[100];
	struct dirent **namelist;
    struct dirent *file = NULL;
    int fd_q;
    int count;
    int i, c;
    char *temp;
    DIR *f;
    char *ext;

    /** 해당 경로 안의 모든 파일들의 dirent 객체를 namelist에 담아주는 함수.*/
    if((count = scandir(ANS, &namelist, NULL, alphasort)) == -1){    
	    fprintf(stderr, "%s directory scan Error\n","./ANS/"); 
	    exit(1);
    }
    //open folder and put file list in q -> pointer 로 구현 가능
    for(c=0; c < count-2; c++){
        i = c+2;
        strcpy(folderlist, ANS);
        strcat(folderlist, "/");
        strcat(folderlist, namelist[i]->d_name); // ./ANS/2-2
        strcat(folderlist, "/"); // ./ANS/2-2/
        

        f = opendir(folderlist);

        strcat(folderlist, namelist[i]->d_name); // ./ANS/2-2/2-2
        
        if(f != NULL){   //forder 열기
            while((file = readdir(f)) != NULL) //file 이름명 q에 넣기, 문제유형 파악
            {
                // 해당 파일의 끝 부분에 .이면 인덱스 값을 알아내고 탐색계속
                ext = strrchr(file->d_name, '.');
                if(ext == NULL) {
                   continue; 
                }

                // file의 확장자가 txt면 확장자 복사.
                if(strcmp(ext, ".txt") == 0) {
                    strcpy(q[c].path, folderlist); // ./ANS/1-1/1-1
                    strcat(q[c].path, ".txt");

                   
                    strcpy(q[c].name, namelist[i]->d_name); //1-1
                    q[c].blank_q = 1;
                    break;
                }

                if(strcmp(ext, ".stdout") == 0) {
                    strcpy(q[c].path, folderlist);// ./ANS/11/11
                    strcat(q[c].path, ".stdout"); // ./ANS/11/11.stdout 

                   
                    strcpy(q[c].name, namelist[i]->d_name);
                    q[c].blank_q = 0;
                    break;
                }
            }
        }    
        closedir(f);
    
        temp = namelist[i]->d_name; // temp = 1-2
        temp = strtok(temp, "-"); //temp = 1
        list[c] = atoi(temp);
    }
    for(i=0; i<count; i++){
        free(namelist[i]);
    }                                                      
    free(namelist);  // 이중포인터 해제 시 각 포인터를 해제해준 다음 자기자신 해제!

    //정렬
    sort(list, count);

   for(c=QStart; c<count; c++) {
        for(k=0; k<BUFFER_SIZE; k++){
          buf[k] = '\0';
        }
        if(q[c].blank_q == 1) {

            if((fd_q = open(q[c].path, O_RDONLY)) < 0) {
                fprintf(stderr,"200 open error :%s\n", q[c].path);
                exit(1);
            }

            /** 각 파일마다 파일 내용 전체사이즈를 알아냄(lseek) 후 read buf 해서 읽어오기*/
            fsize = lseek(fd_q, 0, SEEK_END);
            lseek(fd_q, 0, SEEK_SET);
            read(fd_q, buf, fsize);
            //retToken()로 q[i].answer 입력받은 스트링을 전달
            trim(buf);
                        
            q[c].answer_num = retToken(c, buf);

            close(fd_q);
        }
        else if(q[c].blank_q == 0){
           
            if((fd_q = open(q[c].path ,O_RDONLY|O_CREAT|O_TRUNC)) < 0) {
                    fprintf(stderr, "218 open error %s\n", q[c].path);
                    exit(1);
            }
            
            fsize = lseek(fd_q, 0, SEEK_END);
            lseek(fd_q, 0, SEEK_SET);
            read(fd_q, buf, fsize);

            trim(buf);
            capital(buf);
            
            
            q[c].answer[0] = (char *)malloc(sizeof(char) * (fsize+1));
            trim(buf);

            strcpy(q[c].answer[0], buf);

            q[c].answer_num = 1;
            
            close(fd_q);
        }
   }
   int j;
  // printf("%d\n", c);
  return c;
}

int retToken(int i, char *buf) {
    int j =0;
    char *ptr = strtok(buf, ":");
    int len;
    while(ptr != NULL) {
        len = strlen(ptr) + 1;

        q[i].answer[j] = (char *)malloc(sizeof(char) * len);

        strcpy(q[i].answer[j], ptr);

        ptr = strtok(NULL,":");

        j++;
    }
    return j;
}


//sort 함수
void sort(int *list, int count) {
    char tem_str[30];
    char tem_p[30];
    int tem, idx, temp_b;
    int i, j, key;

    for(i=1; i<count; i++) {
        key = list[i];
        strcpy(tem_str, q[i].name);
        strcpy(tem_p, q[i].path);
        temp_b = q[i].blank_q;

        for(j=i-1; j>=0; j--) {
            if(list[j]>key){
                list[j+1] = list[j];
                strcpy(q[j+1].name , q[j].name);
                strcpy(q[j+1].path, q[j].path);
                q[j+1].blank_q = q[j].blank_q;
            }else
                break;
        }
        list[j+1] = key;
        strcpy(q[j+1].name, tem_str);
        strcpy(q[j+1].path, tem_p);
        q[j+1].blank_q = temp_b;
    }
}

void compare(char *STD, int std_num, int q_num) {
    
    int i,j, k;
    int fd;
    int fsize;
    char filepath[30];
    char forderpath[30];
    char temp[30];
    char filename[30];
    char buf[BUFFER_SIZE];

    /** j는 학생수, q_num 은 학생이 제출한 문제 개수.*/
    for(j=0; j<std_num; j++){
        for(i=QStart; i<q_num; i++){
            
            /*  buf 초기화 -> memset(buf, '\0', BUFFER_SIZE); */
            for(k=0; k<BUFFER_SIZE; k++)
                buf[k] = '\0';

            // 해당 문제가 빈칸문제가 아니면 일일이 열어서, 학생 답지 경로를 복사함.
            if(!q[i].blank_q) {
                strcpy(temp, std[j].path);
                strcat(temp, q[i].name);
                strcat(temp, ".stdout");

               
                if((fd = open(temp, O_RDONLY)) < 0) {
                    fprintf(stderr, "file open error %s %d\n", temp, q[i].blank_q);
                    exit(1);
                }
                fsize = lseek(fd, 0, SEEK_END);
                if(fsize > 1024)
                    break;
                lseek(fd, 0, SEEK_SET);
                read(fd, buf, fsize);

                trim(buf);
                capital(buf);

                /** 학생 답지랑 원래 문제지랑 하나라도 같은 답이 있으면 정답처리
                 *  더 좋게 바꾸고 싶으면 아래로 바꾸면 충분하다.
                 *  str[j].card[i] = 0;
                 *  for (k = 0 ; k < q[i].answer_num ; k++)
                 *  {
                 *      str[j].card[j] = 1; 
                 *      break;
                 *  }
                 * 
                */
                for(k=0; k<q[i].answer_num; k++) {
                    if(strcmp(buf, q[i].answer[k])==0){
                       // printf("%s vs\n%s\n\n", std[j].answer[i], q[i].answer[0]);
                        std[j].card[i] = 1;
                        //printf("정답입니다.\n");
                        break;
                    }
                }

               if(k==q[i].answer_num) {
                     //printf("%s vs\n%s\n\n", std[j].answer[i], q[i].answer[0]);
                     //printf("%s : %s 틀렸습니다.\n", std[j].id, q[i].name);
                     std[j].card[i] = 0;
                    
                }
            }
            
            
            else if(q[i].blank_q) {
                for(k=0; k<q[i].answer_num; k++) {
                    if(strcmp(buf, q[i].answer[k])==0){
                        std[j].card[i] = 1;
                        break;
                    }
                }
                if(k==q[i].answer_num) {
                    
                }
            } 
          
        }printf("%s is finished\n",std[j].id);
    }
}


/** 멀티 스레드로 학생 답 (2번째 문제) 실행*/
void execute_std(int st, int a) {

    pthread_t p_thread[2];
    MultipleArg *multiple_arg;
    multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
    int thr_id;
    int status;
    char temp[30];
    char file[30];
    char error[30];
    int i = 1;

    strcpy(temp, std[st].path); // ./STD/2019000/
    strcat(temp, q[a].name);
    strcat(temp, ".stdexe"); // ./STD/20190000/11.stdexe
    multiple_arg->tmp = temp;

    strcpy(file, std[st].path);
    strcat(file, q[a].name);
    strcat(file, ".stdout"); // ./STD/20190000/11.stdout
    multiple_arg->filename = file;
    
    strcpy(error, error_dir); // ./err
    strcat(error, "/");
    strcat(error, std[st].id); // ./error/20190003/
    mkdir(error, 0777);
    strcat(error, "/");
    strcat(error, q[a].name); // ./error/20190003/11
    strcat(error, "_error.txt"); // ./error/20190003/11._error.txt
    
    multiple_arg->error = error;

    // 학생 파일 실행
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void*)multiple_arg);

    if(thr_id) {
        perror("pthread_creat()");
        return;
    }

    thr_id = pthread_create(&p_thread[1], NULL, control, &p_thread[0]);

    if(thr_id) {
        perror("pthread_creat()");
        return;
    }
    sleep(KILL_TIME);

    pthread_cancel(p_thread[1]);
    // 스레드가 종료될 때까지 기다림.
    pthread_join(p_thread[0],(void**)&status);
    pthread_join(p_thread[1], (void**)&status);
    

    free(multiple_arg);
    return;

}
static void killer(void *arg) {
    
    pthread_t *tids = (pthread_t*)arg;
    //학생파일실행(t_function) 스레드에 최소 요청을 보냄
    pthread_cancel(tids[0]);
}

// 재해석 요지 필요
static void *control(void *arg) {
    
    pthread_cleanup_push(killer, arg);
    do {
        pthread_testcancel();
    } while(1);
    pthread_cleanup_pop(1);
    return (void*)(NULL);
}

void* t_function(void* multiple_arg) {

	int  fd, fd_e;
    int fsize;
    // 스레드 인자로 경로 구조체 전달. (filename, pathname, errno 경로 전달)
    MultipleArg *my_multiple_arg = (MultipleArg*)multiple_arg;
    pid_t pid; // process id
    pthread_t tid; //thread id
    // void* 로 type casting하셔서 넣으시면 됩니다.
    clock_t start, end;
    
    pid = getpid();
    tid = pthread_self();

    //O_TRUNC : 이전 쓰던 데이터 다 날리고 새로 다시 작성
    if((fd = open(my_multiple_arg->filename, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0) 
    {
        /* 해당 파일 열어봤는데  오류 뜨면 error 파일 생성*/
	    fd_e = open(my_multiple_arg->error, O_WRONLY|O_CREAT|O_TRUNC, 0777);

        //★ 표준 오류를 fd_e 파일로 redirection 시킴. 즉, 표준 애러를 fd_e 파일에 전송해줌!
        dup2(fd_e, 2);
        fprintf(stderr, "480 open error %s\n", my_multiple_arg->filename);
	    close(2);
        close(fd_e);
        exit(1);
    }
   
    // 표준 출력을 fd 파일에 저장함.
    if(dup2(fd, 1) != 1) {
        fprintf(stderr,"dup2 fail\n");
        exit(1);
    }
    
    // system 을통해 출력된 내용이 fd에 저장됨
    if(system(my_multiple_arg->tmp)!=0) { //if "temp"'s running time is longer than 5 sec, kill "temp"
        /* 오류가 난 경우, 해당 오류를 fd_e 파일디스크립터에 출력*/
        fd_e = open(my_multiple_arg->error, O_WRONLY|O_CREAT|O_TRUNC, 0777);
        dup2(fd_e, 2);
        fprintf(stderr, "system fail!! %s\n", my_multiple_arg->tmp);

        /*파일 디스크립터 전부 종료료*/
        close(1);
        close(fd);
        close(2);
        close(fd_e);
        return (void*)(NULL);
   }
   else {
        close(1);
        close(fd);
        }
   return (void*)(NULL);
    
}

void creat_score_table(char *ANS,int q_num) {
    
    FILE *fp;
    int i,j,n;
    float b_value, p_value;
    char tmp[30];

    strcpy(tmp, ANS);
    strcat(tmp, "/score_table.csv"); //점수표 만들기.

    //score_table 생성
    printf("score_table.csv file doesn't exist in TREUDIR!\n"
            "1. input blank question and program question's score. ex) 0.5 1\n"
            "2. input all question's score. ex)Input value of 1-1: 0.1\n"
            "select type >>");
    if((fp = fopen(tmp,"w+")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", tmp);
        exit(1);
    }
    int type;
   
    scanf("%d", &type);
    printf("type : %d\n", type);
    
       //value 입력하기 
        switch(type) {

            // 학생 점수 모두 출력.
            case 1 : //문제번호의 점수설정
                printf("Input value of blank question : ");
                scanf("%f", &b_value);
                printf("Input value of prograam question : ");
                scanf("%f", &p_value);
                
                
                //해당 문제의 점수를 설정하는 과정.
                // csv 파일은 , 단위로 셀에 들어감.
                for(i=3; i<q_num; i++) {
                    //빈칸문제
                    if(q[i].blank_q) {
                        q[i].score = b_value;
                        fprintf(fp, "%s,%.2f\n", q[i].name,b_value);
                    }
                    //프로그래밍 문제
                    else if(!q[i].blank_q) {
                        q[i].score = p_value;
                        fprintf(fp, "%s,%.2f\n", q[i].name,p_value);
                    }
                }              
                break;
      
             case 2 :

                //value 입력받기
                for(i=0; i<=q_num; i++) {
                    printf("Input of %s: ", q[i].name);
                    getchar();
                    scanf("%f", &q[i].score);
                    getchar();
                }
                for(i=QStart; i<=q_num; i++) 
                    fprintf(fp, "%s,%.2f\n", q[i].name, q[i].score);
               break;    
        }
        fclose(fp);
    
    return;
}

void creat_score(char *ANS, int std_num, int q_num){

    FILE *fp;
    int i, j;
    float sum = 0;
    float temp;
    char tmp[30];

    strcpy(tmp, ANS);
    strcat(tmp, "/score.csv");


    if((fp = fopen(tmp, "w+")) == NULL) {
        fprintf(stderr,"fopen error for %s\n", "./ANS/score.csv");
        exit(1);
    }

    fprintf(fp, " ,"); //★맨 위에 빈칸 하나 생성 : 명세 양식대로 만드는중

    for(i=QStart; i<q_num; i++) {
        fprintf(fp, "%s,",q[i].name); //시험문제 csv로 구분
    }
    fprintf(fp, "%s\n", "sum"); //마지막 sum csv에 넣기.

    for(j=0; j<std_num; j++) {
        
        sum=0;
        fprintf(fp, "%s,", std[j].id); //학번 넣기
        for(i=QStart; i<q_num; i++) { //각 문제당 학생들의 성적기입.
            if(std[j].card[i] == 1){  //학생이 맞추었으면 점수 주기.
                sum += q[i].score;
                fprintf(fp, "%.2f,", q[i].score);
            }
            else // 학생이 빈칸으로 제출하였으면 0.00점 처리
                fprintf(fp, "0.00,");
        }
        fprintf(fp, "%.2f\n", sum); //학생으 마지막 총합 처리.
    }
    fclose(fp);
}

int creat_std_table(char *STD, int q_num){
//STD 학번넣기 std struct에 넣기
    
	struct dirent **stdlist;
    struct dirent **filelist;
    int count_s, count_f;
    int i, j, k, s, fd;
    char buf[BUFFER_SIZE];
    int length;
    char temp[30];
    char err[30];

    // STD 파일의 모든 파일/디렉토리 탐색.
    if((count_s = scandir(STD, &stdlist, NULL, alphasort)) == -1){    
	    fprintf(stderr, "%s directory scan Error\n", "./STD/");
	    exit(1);
    }
    for(s=0; s<count_s-2; s++) {
        i = s+2;
        strcat(std[s].id, stdlist[i]->d_name);

        //printf("stdid : %s\n", std[s].id);
        for(j=QStart; j<q_num; j++) {
                //sprintf(std[s].path, "%s/%s/", STD, sts[s].id) <- 대체 가능
                //그리고 for 문 밖으로 빼야됨.
                strcpy(std[s].path, STD);
                strcat(std[s].path, "/");
                strcat(std[s].path, std[s].id); //./STD/20162467
                strcat(std[s].path, "/"); // ./STD/20162467/

                /** 아래 코드 다음으로 확 줄이기 가능.  (sprintf 쓸 때는 꼭 \0, \n 붙일껏)
                 * char *ptr = std[s].path + strlen(std[s].path);
                 *  if (q[j].blank_q)
                 * {
                 *      sprintf(ptr, "%s.txt\0", q[j].name);
                 *      .... 
                 * }
                */
                if(q[j].blank_q == 1) {//빈칸문제일 경우
                    strcpy(temp, std[s].path);
                    strcat(temp, q[j].name); // ./STD/20162467/1-1
                    strcat(temp, ".txt");

                    if((fd = open(temp, O_RDONLY|O_CREAT)) < 0) {
                        fprintf(stderr,"open error for %s\n", std[s].path);
                        exit(1);
                    }

                    // 빈칸문제의 학생 답안을 파일에서 긁어옴.
                    length = lseek(fd, 0, SEEK_END); // 파일 전체 크기
                    lseek(fd, 0, SEEK_SET);
	                read(fd, buf, length);
                    
                    /** 
                     *  answer 문자열 객체를 length만큼 할당해줌  
                     * 로직이 꼬여있는것 같음
                     *  ↓
                     * int idx = trim(buf);
                     * std[s].answer[j] = (char*)malloc(idx+1);
                     * buf[idx] = '\0';
                     * strcpy(std[s].anser[j], buf);
                     * 
                     */
                    std[s].answer[j] = (char *)malloc(length+1); 
                    trim(buf);
                    strcpy(std[s].answer[j],buf); 

		            for(k=0; k<BUFFER_SIZE; k++){
                        buf[k] = '\0';
                    }

                    close(fd);
                }
                else if(q[j].blank_q == 0){//프로그래밍 문제일 경우 -> 필요없으니 패스

//                    execute_std(s,j);
                    continue;
                }

            }
       // printf("280\n");
    }
   // printf("282\n");
   /* for(i=0; i<count_s; i++){
        free(stdlist[i]);
    }*/
    free(stdlist);
    return s;
}

/** 
 *  위에서 사용하려면 함수를 void->int형으로 바꾸고 index를 return해주는것이 좋아보임.
 * 문자열 끝 제거 -> strcpy(s,s+1) 을 해줌으로서 \t0, \n 을 없애버림.*/
void trim(char* s) {
    char t[MAX_STR_LEN];
    char *end;
          
        
      
	for(; *s!= '\0'; s++){
        if(*s == '\n'){
            strcpy(s, s+1);
            s--;
        }
	if(*s == '\0'){
            strcpy(s, s+1); 
            s--;
        }
	}

}

/* 대문자화*/
void capital(char *s) {

        int i ;
        
        for(i=0; s[i] == '\0'; i++) {

           if(s[i]>='a' && s[i]<= 'z')
                s[i] = s[i]-32;
        }
}


    


////////////////////////////////
///q[i].answer[j]왜 안들어가는지 확인///
//정렬했을때 앞에 빈 애들 처리해주기//
///////////////////////////////
