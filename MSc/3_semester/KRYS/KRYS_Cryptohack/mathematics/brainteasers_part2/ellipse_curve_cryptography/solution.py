from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from hashlib import sha1
import random
from Crypto.Util.number import long_to_bytes, bytes_to_long, inverse

D = 529
p = 173754216895752892448109692432341061254596347285717132408796456167143559


def point_addition(P, Q):
    x = (P[0] * Q[0] + D * P[1] * Q[1]) % p
    y = (P[0] * Q[1] + P[1] * Q[0]) % p
    return [x, y]


def point_subtraction(P, Q):
    x = (P[0] * Q[0] - D * P[1] * Q[1]) % p
    y = (P[1] - x * Q[1]) * inverse(Q[0], p) % p
    return [x, y]


def scalar_multiplication(P, n):
    Q = [1, 0]
    while n > 0:
        if n % 2 == 1:
            Q = point_addition(Q, P)
        P = point_addition(P, P)
        n = n//2
    return Q


def gen_keypair():
    private = random.randint(1, p-1)
    public = scalar_multiplication(G, private)
    return (public, private)


def gen_shared_secret(P, d):
    return scalar_multiplication(P, d)[0]


# ================ #
# Curve parameters #
# ================ #
G = [29394812077144852405795385333766317269085018265469771684226884125940148, 94108086667844986046802106544375316173742538919949485639896613738390948]
A = [155781055760279718382374741001148850818103179141959728567110540865590463, 73794785561346677848810778233901832813072697504335306937799336126503714]
B = [171226959585314864221294077932510094779925634276949970785138593200069419, 54353971839516652938533335476115503436865545966356461292708042305317630]
Encrypted_flag = {'iv': '64bc75c8b38017e1397c46f85d4e332b', 'encrypted_flag': '13e4d200708b786d8f7c3bd2dc5de0201f0d7879192e6603d7c5d6b963e1df2943e3ff75f7fda9c30a92171bbbc5acbf'}

D_sqrt = 23

u_g = (-23 * G[1] + G[0]) % p
u_a = (-23 * A[1] + A[0]) % p

print(u_g)
print(u_a)

n_A = 85659350935432836744105386596354218808820976897571304400328185351959247

shared_secret = gen_shared_secret(B, n_A)
key = sha1(str(shared_secret).encode('ascii')).digest()[:16]
cipher = AES.new(key, AES.MODE_CBC, bytes.fromhex(Encrypted_flag['iv']))
flag = cipher.decrypt(bytes.fromhex(Encrypted_flag['encrypted_flag']))
print(flag)