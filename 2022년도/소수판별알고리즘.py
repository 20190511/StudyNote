#소수 판별 알고리즘
""" 약수는 대칭을 이루기 떄문에, x의 중간값(root(x)+1) 까지만 확인해주면 된다. """
def is_prime_number(x):
    for i in range(2, int(pow(x,0.5) + 1)):
        if x % i == 0:
            return False
    return True
    
    
print(is_prime_number(6))
