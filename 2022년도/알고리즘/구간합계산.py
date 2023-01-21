#구간 합 빠르게 계산하기.
"""
arr = [10,20,30,40,50] 이 있을 때, arr[3:5] 구간의 합을 구하고싶으면
    접두사 합(Prefix Sum): P=[0,10,30,60,100,150] 을 구한다.
    
    P[5] - P[2] = 150-30 로 구할 수 있다.
"""

n = 5 
data = [10,20,30,40,50]
sums = 0 
prefix_sums = [0]
for i in data:
    sums += i
    prefix_sums.append(sums)
    
left,right =3,5 
print(prefix_sums[right]-prefix_sums[left-1])
