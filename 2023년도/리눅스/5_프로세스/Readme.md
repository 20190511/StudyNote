# 프로세스 
## char **environ 전역변수는 char 포인터를 넣는다! 그래서 재호출을 하지않고 char값만 바꿔도 environ도 갱신된다.
### 아래와 같이 glob_var 변수의 포인터를 넣는 것을 통해, syscpy(glob_var, "바꿀값"); 만 적고 putenv()를 재호출하지 않아도 값이 바뀐다.
 ex)![image](https://user-images.githubusercontent.com/70988272/218240277-7cc128a4-7065-4576-9542-4b5bbce48a9e.png)
