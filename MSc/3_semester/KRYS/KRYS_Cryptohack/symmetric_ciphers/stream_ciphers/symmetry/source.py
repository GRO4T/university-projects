import requests

URL = "https://aes.cryptohack.org/symmetry/encrypt"

flag = "crypto{"

ct = "c39eb62fb8f3a90814c7d52d88b00bb0966579922518366ffc53a6af2fe3a198946021680a8a60e4e2aa7375fc75d82b64"
iv = ct[:32]
c = ct[32:]

print(iv)
print(c)


i = len(flag)
while "}" not in flag:
    print(f"{flag=}")
    print(f"{i=}")
    for b_code in range(256):
        b = hex(b_code).ljust(4, "0")[2:]
        # print(f"{b=}")

        pt = flag.encode().hex() + b

        # print(f"{pt=}")

        r = requests.get(f"{URL}/{pt}/{iv}")
        ct2 = r.json()["ciphertext"]

        # print(f"{ct2}")
        # print(f"{c}")
        
        index = 2*i
        if c[index:index+2] == ct2[index:index+2]:
            flag += chr(b_code)
            i += 1
            break
# crypto{\x03fb_15_5ymm37r1c4l_!!!11!}