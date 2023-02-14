## GCC 명령어 및 옵션
gcc <옵션> <파일명>   
### C 컴파일 빌드 : 전처리 단계(.c) -> 컴파일 단계 (어셈블리어 .s) -> 어셈블 단계 (오브젝트파일 .o) -> 링크 단계 (실행파일 .out)  
gcc -o (FILE) : 지정한 FILE 로 출력파일을 만든다. (링크 단계까지 해주고 실행파일 생성)  
gcc -c : 컴파일과 어셈블만 수행한다. (링크 실행을 안함.)  
gcc -g : 실행 파일을 생성할 때, 디버거를 위한 변수테이블 생성.  
<br>  
gcc -I : 헤더파일을 검색할 디렉토리명  
gcc -L : 라이브러리를 검색할 디렉토리명  
gcc -l : 링크할 라이브러리   
![image](https://user-images.githubusercontent.com/70988272/216768764-d6d49802-0876-4885-b57c-b152989e416a.png)
---

## 7. 쓰레드의 개념
---
![image](https://user-images.githubusercontent.com/70988272/218644652-2243660e-7c1d-4cac-a97d-687d37bf21f1.png)

![image](https://user-images.githubusercontent.com/70988272/218644686-af473bc6-e671-4139-9848-43361d6b3367.png)

![image](https://user-images.githubusercontent.com/70988272/218644724-de67eae8-1313-4bac-8dbd-04c19f23db58.png)

![image](https://user-images.githubusercontent.com/70988272/218644757-5ec757fc-c2a9-4984-a6d6-5292311ef057.png)

![image](https://user-images.githubusercontent.com/70988272/218644780-33c0d5b0-5a4d-4af9-8700-407635b0f045.png)
