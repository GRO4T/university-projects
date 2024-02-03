import numpy as np

v = [
    np.array([4, 1, 3, -1]),
    np.array([2, 1, -3, 4]),
    np.array([1, 0, -2, 7]),
    np.array([6, 2, 9, -5]),
]

u = [0] * 4
m = [[0] * 4] * 4

u[0] = v[0]

for i in range(1, len(v)):
    for j in range(i):
        m[i][j] = u[j] * np.dot(v[i], u[j]) / np.dot(u[j], u[j])
    u[i] = v[i] - sum((m[i][j] for j in range(i)))

print(u)
