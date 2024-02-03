import numpy as np


def size(vec: np.array) -> float:
    return np.sqrt(np.inner(vec, vec))


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


v1 = np.array([846835985, 9834798552])
v2 = np.array([87502093, 123094980])
v1, v2 = gaussian_reduction(v1, v2)

print(f"v1={v1} v2={v2}")
print(f"dot(v1,v2)={np.inner(v1, v2)}")
