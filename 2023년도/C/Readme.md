# 현재 생각하는 C언어 약점 정리 (이론 파트랑 실제파트랑 분리)
---
## 이론
0. 입출력 예제
0. 파일 분리 개념 (헤더 / 오브젝트파일 / C파일 분리) 
0. define
0. int main (int argc, int* argv) 
0. 각 타입 별 메모리 재정리 (double, float, long long 등 각 메모리크기) (해결)
0. malloc 같은 void* 타입 포인터 정리 (형변환 여부) (해결)
0. switch 문 (해결)
0. char[] 과 char* 의 비교 (해결)
0. Const 개념! Static 개념! extern 개념! (해결)
0. typedef 와 enum (해결)
0. 구조체 변수와 포인터의 관계 (해결)
0. 함수 타입 포인터 + 함수 매개변수 포인터 정리 (해결)
0. 가변함수 (해결)
---
## 실기
1. 자료구조 구현 (연결리스트, 힙, 트리(순회포함), LCS(DP) 등등)
2. 실제 프로젝트 함수들 정리 (함수명/함수사용 방식/ 함수 사용 예제 순으로 정리) + C언어 STL 함수
3. 파일 분리
---
# 이론 정리
0. Switch : ![switch](https://user-images.githubusercontent.com/70988272/216491871-adbbd184-2bdc-4466-abd4-4cfca1acd6ec.JPG)
0. 타입별 사이즈 + 형식문자 : ![형식문자+사이즈](https://user-images.githubusercontent.com/70988272/216497050-1bee0d9d-334f-4a73-bd48-18d1ca632e26.JPG)

0. char[] 와 char* 비교 : ![image](https://user-images.githubusercontent.com/70988272/216499594-0c7f00d3-56be-4edd-81ca-d5bcf5e828b0.png)

0. Const, Static, Extern : ![image](https://user-images.githubusercontent.com/70988272/216510650-e62e895a-2970-4b85-9e41-3125a95cd55d.png)

0. typedef 와 enum : ![image](https://user-images.githubusercontent.com/70988272/216519136-f73b7317-9aeb-4388-8ee8-c07ebc4d56a6.png)

0. malloc 같은 형 변환 : ![image](https://user-images.githubusercontent.com/70988272/216532933-742311bb-6a9c-4517-b96d-047061746727.png) + [형변환 꼭 해야하는가?!](https://untitle-ssu.tistory.com/69)

0. 구조체 (포인터와 개념) :![image](https://user-images.githubusercontent.com/70988272/216589263-2725ff81-f857-4125-ae72-eb0b60c686ea.png)

0. 다차원 배열과 포인터+매개변수전달 방식 : ![image](https://user-images.githubusercontent.com/70988272/216599492-4ca49994-bb61-4429-9c03-24b924f2e8a4.png)

0. 파일처리 : ![image](https://user-images.githubusercontent.com/70988272/216636588-6ebd5ce4-88a6-4da7-a22c-7b2743da8d98.png)

0. 가변함수 : ![image](https://user-images.githubusercontent.com/70988272/216602537-f11846d4-699b-460c-94c8-6a3a6b498da1.png)

0. 형식문자 변환 :  
![image](https://user-images.githubusercontent.com/70988272/219216103-b93a72d2-1084-439a-8936-d6350667aa13.png)

