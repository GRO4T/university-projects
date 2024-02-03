#!/usr/bin/env python3

from Crypto.Util.number import getPrime, bytes_to_long

FLAG = b"crypto{?????????????????????????}"

d=0
m = bytes_to_long(FLAG)

def get_huge_RSA():
    global d
    p = getPrime(1024)
    q = getPrime(1024)
    N = p*q
    phi = (p-1)*(q-1)
    while True:
        d = getPrime(256)
        e = pow(d,-1,phi)
        if e.bit_length() == N.bit_length():
            break
    return N,e


N, e = get_huge_RSA()
c = pow(m, e, N)

print(f"d = {hex(d)}")
print(f"m = {hex(m)}")
print(f'N = {hex(N)}')
print(f'e = {hex(e)}')
print(f'c = {hex(c)}')
