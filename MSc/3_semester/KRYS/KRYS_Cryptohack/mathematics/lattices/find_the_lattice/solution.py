import numpy as np

from Crypto.Util.number import inverse, bytes_to_long, long_to_bytes


def decrypt(q, h, f, g, e):
    a = (f * e) % q
    m = (a * inverse(f, g)) % g
    return m

q, h = (7638232120454925879231554234011842347641017888219021175304217358715878636183252433454896490677496516149889316745664606749499241420160898019203925115292257, 2163268902194560093843693572170199707501787797497998463462129592239973581462651622978282637513865274199374452805292639586264791317439029535926401109074800)
u = np.array([1, h])
v = np.array([0, q])
e = 5605696495253720664142881956908624307570671858477482119657436163663663844731169035682344974286379049123733356009125671924280312532755241162267269123486523

def size(vec: np.array) -> float:
    x = np.inner(vec, vec)
    return np.sqrt(float(x))

def gaussian_reduction(v1: np.array, v2: np.array) -> (np.array, np.array):
    while True:
        if size(v2) < size(v1):
            x = v2
            v2 = v1
            v1 = x
        m = round(np.inner(v1, v2) / np.inner(v1, v1))
        if m == 0:
            return (v1, v2)
        v2 = v2 - m * v1


u, v = gaussian_reduction(u, v)

f, g = u

m = decrypt(q, h, f, g, e)
print(long_to_bytes(m))

