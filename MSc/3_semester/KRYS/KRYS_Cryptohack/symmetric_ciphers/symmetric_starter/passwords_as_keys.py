from Crypto.Cipher import AES
import hashlib
import random
from base64 import b64decode


# /usr/share/dict/words from
# https://gist.githubusercontent.com/wchargin/8927565/raw/d9783627c731268fb2935a731a618aa8e95cf465/words
with open("./words.txt") as f:
    words = [w.strip() for w in f.readlines()]
keyword = random.choice(words)

KEY = hashlib.md5(keyword.encode()).digest()

ENCRYPTED_FLAG = "c92b7734070205bdf6c0087a751466ec13ae15e6f1bcdd3f3a535ec0f4bbae66"



def decrypt(ciphertext, password_hash):
    ciphertext = bytes.fromhex(ciphertext)
    key = password_hash

    cipher = AES.new(key, AES.MODE_ECB)
    try:
        decrypted = cipher.decrypt(ciphertext)
    except ValueError as e:
        return {"error": str(e)}

    return {"plaintext": decrypted.hex()}

for w in words:
    password_hash = hashlib.md5(w.encode()).digest()
    r = decrypt(ENCRYPTED_FLAG, password_hash)
    hex = r["plaintext"]
    try:
        plaintext = bytearray.fromhex(hex).decode()
        print(plaintext)
        if "crypto" in plaintext:
            print(f"Got the flag {r['plaintext']}!: {w} ({password_hash})")
    except:
        pass