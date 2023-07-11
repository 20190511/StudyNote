#include "types.h"
#include "fcntl.h"
#include "stat.h"
#include "user.h"

#ifndef NULL
#define NULL 0
#endif

char userID[16][32];
char pwdID[16][32];
char** glob_argv;

void get_user_list() {
    int fd, i;
    char login_buf[64];

    fd = open("list.txt", O_RDONLY);
    if (fd < 0) {
        printf(1, "open error for %s\n", "list.txt");
        exit();
    }

    for (i = 0 ; i < 16 ; i++) {
        char* char_ptr = login_buf;
        while(read(fd, char_ptr, 1) > 0) {
            if (*char_ptr == '\n') {
                *char_ptr = '\0';
                break;
            }
            else if (*char_ptr == '\0') 
                break;
            char_ptr++;
        }

        char* tkn = strchr(login_buf, ' ');
        if (tkn == NULL)
            continue;
        *tkn++ = '\0';
        strcpy(userID[i], login_buf);
        strcpy(pwdID[i], tkn);
        //printf(1, "%s:%s\n", userID[i], pwdID[i]);
    }
}

int 
check_idpw(){
    char id[32], pw[32];
    int pid, status, i;
    printf(1, "Username: ");
    while (gets(id, 32) == NULL) {
        printf(1, "Username: ");
    }
    id[strlen(id)-1] = '\0';
    printf(1, "Password: ");
    while (gets(pw, 32) == NULL) {
        printf(1, "Password: ");
    }
    pw[strlen(pw)-1] = '\0';
    
    for (i = 0 ; i < 16 ; i++) {
        if(!strcmp(userID[i], id) && !strcmp(pwdID[i], pw)) {
           if ((pid = fork()) < 0) {
                printf(1, "fork error");
                exit();
           }
           else if (pid == 0) {
                exec("sh", glob_argv);
                printf(1, "sh error\n");
                exit();
           }
           while((status = wait()) >= 0 && status != pid) {
                printf(1, "zombie!!\n"); 
           }
        }
    }

    printf(1, "wrong user info, again\n");
    return check_idpw();
}

int main (int argc, char* argv[]) {
    glob_argv = argv;
    get_user_list();
    check_idpw();
    return 0;
}