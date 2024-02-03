x = 5
while True:
    print(f"{x=}")
    if x % 5 == 2 and x % 11 == 3:
        a = x % 935
        print(f"Solution to x = a mod 935: x={x} a={a}")
        break
    x += 17
