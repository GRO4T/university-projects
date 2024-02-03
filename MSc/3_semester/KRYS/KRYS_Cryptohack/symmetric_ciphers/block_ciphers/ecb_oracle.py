import os
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

import requests


URL = "https://aes.cryptohack.org/ecb_oracle/"

# FLAG = "crypto{" + "0" * 24 + "}"
# FLAG = "crypto{p3n6u1n5_" + "0" * 15 + "}"
FLAG = "crypto{p3n6u1n5_h473_3cb" + "0" * 7 + "}"


i = 7

print(FLAG[:i])

# stage 1
# for i in range(7, 16):
#     for b in range(256):
#         print(".")
#         flipping_byte = hex(b).ljust(4, "0")
#         padding = 16 - i - 1
#         known_flag_part = FLAG[:i]
#         print(f"{padding} {known_flag_part}")
#         plaintext = "05" * padding + known_flag_part.encode().hex() + flipping_byte[2:] + "05" * padding
#         print(plaintext)
#         r = requests.get(URL + "encrypt/" + plaintext)
#         ciphertext = r.json()["ciphertext"]
#         first_block = ciphertext[0:32]
#         second_block = ciphertext[32:64]
#         if first_block == second_block:
#             FLAG = FLAG[:i] + bytearray.fromhex(flipping_byte[2:]).decode() + FLAG[i+1:]
#             print(FLAG)
#             break


# stage 2
for i in range(8, 16):
    for b in range(256):
        print(".")
        flipping_byte = hex(b).ljust(4, "0")
        padding = 16 - i - 1
        known_flag_part = FLAG[:16+i]
        print(f"{padding} {known_flag_part}")
        plaintext = "05" * padding + known_flag_part.encode().hex() + flipping_byte[2:] + "05" * padding
        print(plaintext)
        r = requests.get(URL + "encrypt/" + plaintext)
        ciphertext = r.json()["ciphertext"]
        second_block = ciphertext[32:64]
        fourth_block = ciphertext[96:128]
        if second_block == fourth_block:
            x = 16 + i
            FLAG = FLAG[:x] + bytearray.fromhex(flipping_byte[2:]).decode() + FLAG[x+1:]
            print(FLAG)
            break