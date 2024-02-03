from pwn import *
import json
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib
import re


r = remote("socket.cryptohack.org", 13371, level="debug")
# r = remote("socket.cryptohack.org", 13371)


def json_recv():
    line = r.recvline()
    match = re.search("{.*}", line.decode())
    return json.loads(match.group(0))

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)


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


intercept_alice = json_recv()
print(intercept_alice)
intercept_alice["A"] = "0x1"

json_send(intercept_alice)

intercept_bob = json_recv()
intercept_bob["B"] = "0x1"

json_send(intercept_bob)

encrypted_flag = json_recv()

pt = decrypt_flag(1, encrypted_flag["iv"], encrypted_flag["encrypted_flag"])
print(f"{pt=}")