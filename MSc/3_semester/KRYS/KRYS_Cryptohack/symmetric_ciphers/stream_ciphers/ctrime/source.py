from Crypto.Cipher import AES
from Crypto.Util import Counter
import zlib
import os
import requests

url = "https://aes.cryptohack.org/ctrime/encrypt/" 
flag = "crypto{"
ct = "5653d75433fc08c80ab48ee43416158521e984d2a202f8df335c5c18f09181f2cf22"

while "}" not in flag:
    print(f"{flag=}")
    for b_code in range(256):
        b = hex(b_code).ljust(4, "0")[2:]
        # print(f"{b=}")
        pt = flag.encode().hex() + b
        r = requests.get(f"{url}{pt}")
        ct2 = r.json()["ciphertext"]

        # print("--------------")
        print(ct)
        print(ct2)
        # print("--------------")

        if len(ct) == len(ct2):
            flag += chr(b_code)
            break
        