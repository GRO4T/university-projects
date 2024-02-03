from pwn import *
import json
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib
import re

def is_pkcs7_padded(message):
    padding = message[-message[-1]:]
    return all(padding[i] == len(padding) for i in range(0, len(padding)))


def decrypt_flag(shared_secret: int, iv: str, ciphertext: str):
    # Derive AES key from shared secret
    sha1 = hashlib.sha1()
    sha1.update(str(shared_secret).encode('ascii'))
    key = sha1.digest()[:16]
    # Decrypt flag
    ciphertext = bytes.fromhex(ciphertext)
    iv = bytes.fromhex(iv)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = cipher.decrypt(ciphertext)

    if is_pkcs7_padded(plaintext):
        return unpad(plaintext, 16).decode('ascii')
    else:
        return plaintext.decode('ascii')

p = 0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff
g = 0x02
A = 0xfb91010c483176c69753b47dba5210b9bb4cf31b6337bc2055ecde339b2bf8fab16830cc0a5252bbc2ba2a6aa23e530dcf387fa1ead61dc97a155fc50749670e611dbf63916fb9cf7ae456d8b47a18bf64bfcdec5f949076f15509dbf506a8be6bdf1c1a7404e2819c388dda07d95e0f199d7a5b8ead0bf4665f2c312e64d57ba30bc43b35acf443846d045d75491cbfa976f2f9771c5f6ba781712b4f8693f1416ba6d0acc021deb82b16c07463bcf38fac6e0e6553128c798a510a25c3801a
B = 0xe976cea967115605dc4ae035cfe4622cbfb8dab890d77f04df41348eec0ca418a452520ef2feef0771d997adfe2a0506d426422c7a3601706f6ad4dcda985ed6bc6b50fe86110298143d19a29c4f44d6fa7d11e39e253d591cd24ac4f5d4a2594ead0c83a6ad4f8cb5f5be12b9f2fed7d894a26c886d802c3a3650ba58ea542f895213a78eb1cd59b1b49a1e5701cc8d4a82f96f01fe771bb28fe3413da5d0891deb9d0f8dc0fc6107e56e5234864b6d0da99828c9c859ef25e2792b7860be11
iv = "a81fb81a12ccf9c7e809716b65395221"
encrypted = "5a35c64121aa5a6c066cdbf99547be4fa85895e7f983b377e573536e9ee4703b10042a71ea666f4bdae0d6be81ed769a"

b = (B // g) % p
a = (A // g) % p

print("---------------------")
print(f"{p=}")
print("---------------------")
print(f"{g=}")
print("---------------------")
print(f"{a=}")
print("---------------------")
print(f"{b=}")
print("---------------------")
print(f"{A=}")
print("---------------------")
print(f"{B=}")
print("---------------------")

# https://crypto.stackexchange.com/questions/41096/diffie-hellman-key-exchange-using-addition-instead-of-multiplication
# B = b * g
# A = a * g
# S = A * b = a * g * b = B * a

S = ((A * B) // g) % p

pt = decrypt_flag(S, iv, encrypted)
print(f"{pt=}")