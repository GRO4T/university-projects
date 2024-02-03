def gcd(a, b):
    while b != 0:
        t = b
        b = a % b
        a = t
    return a

a = 66528
b = 52920
print(gcd(a, b))