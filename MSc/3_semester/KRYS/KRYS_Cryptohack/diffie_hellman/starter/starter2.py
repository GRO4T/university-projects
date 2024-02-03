def is_primitive_element(g, p) -> bool:
    nums = set()
    for i in range(1, p):
        x = pow(g, i, p)
        if x in nums:
            return False
        nums.add(x)
    return True

p = 28151
for g in range(2, p):
    if is_primitive_element(g, p):
        print(f"found {g=}")
        break
