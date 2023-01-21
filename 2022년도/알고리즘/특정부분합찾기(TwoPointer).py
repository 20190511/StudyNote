# 투 포인터
#시작점과 끝 점으로 2개의 기준으로 데이터 가져오기.
""" 수열 중 합이 M인 부분연속수열 개수를 구하고 싶은데 O(N)으로 구하고 싶은 경우 
 1. 시작점과 끝점 (start, end) 가 인덱스(0) 을 가리키게 한다.
 2. 부분합이 M과 같다면 Count, 부분합<M : end+=1, 부분합>M : start += 1 
 3. 모든 경우를 확인할 때 까지 2~4번 반복
"""


def two_pointer(arr,target):
    end = 0
    count = 0
    sums = 0 
    for start in range(len(arr)):
        while sums<target and end<len(arr):
            sums += arr[end]
            end += 1 
        if sums == target:
            print(start+1,end)
            count += 1 
        sums -= arr[start]
    return count 
    
arrs = [1,2,3,2,5]
target = 5 
print(two_pointer(arrs,target))
