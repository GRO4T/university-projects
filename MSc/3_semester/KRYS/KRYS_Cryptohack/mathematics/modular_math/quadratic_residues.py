p = 29
ints = [14, 6, 11]

for x in ints:
    has_sqrt = False
    for i in range(1, p):
        if i**2 % p == x:
            print(f"square root modulo {p} of {x} is {i}")
            has_sqrt = True
            break
    if not has_sqrt:
        print(f"{x} has no square root modulo {p}")