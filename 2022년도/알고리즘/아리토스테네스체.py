""" 특정한 수의 범위에 존재하는 모든 소수 찾기 """
#에라토스테네스 체 O(NloglogN) if N = 1,000,000 이면 약 4,000,000만번만 연산하면 풀 수 있다.
# 단 메모리가 많이 필요하다.
n = 1000
array = [True for _ in range(n+1)]

for i in range(2, int(pow(n,0.5))+1):
    if array[i] == True:
        j = 2
        while i*j < n:
            array[i*j] = False
            j += 1 
            
for i in range(2, n+1):
    if array[i]:
        print(i, end=" ")
