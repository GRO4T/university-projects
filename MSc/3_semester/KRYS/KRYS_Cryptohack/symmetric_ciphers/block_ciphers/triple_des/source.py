import os
from Crypto.Cipher import DES3
from Crypto.Util.Padding import pad


IV = os.urandom(8)
FLAG = "a" * 8


def xor(a, b):
    # xor 2 bytestrings, repeating the 2nd one if necessary
    return bytes(x ^ y for x,y in zip(a, b * (1 + len(a) // len(b))))



def encrypt(key, plaintext):
    try:
        key = bytearray.fromhex(key)
        plaintext = bytes.fromhex(plaintext)
        plaintext = xor(plaintext, IV)

        cipher = DES3.new(key, DES3.MODE_ECB)
        ciphertext = cipher.encrypt(plaintext)
        ciphertext = xor(ciphertext, IV)

        return {"ciphertext": ciphertext.hex()}

    except ValueError as e:
        return {"error": str(e)}


def encrypt_flag(key):
    return encrypt(key, pad(FLAG.encode(), 8).hex())

print(FLAG.encode().hex())
print(encrypt_flag(os.urandom(16).hex()))