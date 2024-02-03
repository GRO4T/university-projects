from pwn import *
from Crypto.Util.number import isPrime, inverse
import json
import random

r = remote("socket.cryptohack.org", 13403)

q = int(r.recvline().decode().split('Prime generated: ')[1][1:-2], 16)
print("q =", q)
n = q ** 2
phi_n = q * (q - 1)
g = pow(2, q - 1, n)

k = ((g - 1) // q) % q

send = {'g': hex(g), 'n': hex(n)}
send = json.dumps(send).encode()
r.sendline(send)
g_x = int(r.recvline().decode().split('Send integers (g,n) such that pow(g,q,n) = 1: Generated my public key: ')[1][1:-2], 16)
print("g_x =", g_x)
x = ((g_x - 1) // q) * inverse(k , q) % q

send = {'x': hex(x)}
send = json.dumps(send).encode()
r.sendline(send)

print(r.recvline())
