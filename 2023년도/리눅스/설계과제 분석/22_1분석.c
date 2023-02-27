
/** 1. strtok 는 delmiter 앞이 없으면 뒷 문자를 첫 문자로 return 한다. **/
char path[] = "as/home/junhyeong/go2";        // 첫 strtok 때 as 가 들어감
char path2[] = "/home/junhyeong/go2";         // 첫 strtok 때 home 이 들어감
//아래처럼 돌리면 안돌아감. <-- strtok에 할당되어진 문자열을 넣어야지 썡 문자열을 박아서 포인터로 가리키는 형태로 적으면 안됨.
//char *path = "as/home/junhyeong/go2";
