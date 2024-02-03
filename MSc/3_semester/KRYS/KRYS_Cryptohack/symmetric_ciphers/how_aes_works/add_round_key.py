state = [
    [206, 243, 61, 34],
    [171, 11, 93, 31],
    [16, 200, 91, 108],
    [150, 3, 194, 51],
]

round_key = [
    [173, 129, 68, 82],
    [223, 100, 38, 109],
    [32, 189, 53, 8],
    [253, 48, 187, 78],
]


def matrix2bytes(matrix):
    """Converts a 4x4 matrix into a 16-byte array."""
    bytes = []
    for row in matrix:
        bytes.extend(row)
    return "".join(map(lambda x: chr(x), bytes))


def add_round_key(s, k):
    b = []
    for i in range(4):
        row = []
        for j in range(4):
            row.append(s[i][j] ^ k[i][j])
        b.append(row)
    return matrix2bytes(b)


print(add_round_key(state, round_key))
