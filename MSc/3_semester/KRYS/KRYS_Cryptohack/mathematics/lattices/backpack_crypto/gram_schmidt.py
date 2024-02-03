import numpy as np

def gram_schmidt(v):
    n = len(v)
    u = [0] * n 
    m = [[0] * n] * n

    u[0] = v[0]

    for i in range(1, len(v)):
        for j in range(i):
            m[i][j] = u[j] * np.dot(v[i], u[j]) / np.dot(u[j], u[j])
        u[i] = v[i] - sum((m[i][j] for j in range(i)))

    return np.array(u).astype(int)
