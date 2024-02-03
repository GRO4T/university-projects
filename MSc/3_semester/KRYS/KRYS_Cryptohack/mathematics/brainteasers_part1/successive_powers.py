def is_prime(n):
    for i in range(2, n):
        if (n % i) == 0:
            return False
    return True


powers = [588, 665, 216, 113, 642, 4, 836, 114, 851, 492, 819, 237]
basis = [x for x in range(100, 1000) if is_prime(x) and x > max(powers)]
for p in basis:
    for x in range(1, p):
        for i, power in enumerate(powers):
            if i == len(powers) - 1:
                print("crypto{", p, ",", x, "}", sep="")
            elif not (x * power) % p == powers[i + 1]:
                break
