import numpy as np

basis = np.array([[6, 2, -3], [5, 1, 4], [2, 7, 1]])


def det(mat: np.array) -> float:
    aei = mat[0][0] * mat[1][1] * mat[2][2]
    bfg = mat[1][0] * mat[2][1] * mat[0][2]
    cdh = mat[2][0] * mat[0][1] * mat[1][2]
    ceg = mat[2][0] * mat[1][1] * mat[0][2]
    bdi = mat[1][0] * mat[0][1] * mat[2][2]
    afh = mat[0][0] * mat[2][1] * mat[1][2]

    return aei + bfg + cdh - ceg - bdi - afh


print(det(basis))
