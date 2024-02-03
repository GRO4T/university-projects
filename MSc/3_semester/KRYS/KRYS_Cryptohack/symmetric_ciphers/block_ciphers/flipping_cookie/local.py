import os
from Crypto.Util.Padding import pad, unpad
from Crypto.Cipher import AES
from datetime import datetime, timedelta
from pwn import xor


KEY = os.urandom(16)
FLAG = "crypt{abcdef}"


def check_admin(cookie, iv):
    cookie = bytes.fromhex(cookie)
    iv = bytes.fromhex(iv)

    try:
        cipher = AES.new(KEY, AES.MODE_CBC, iv)
        decrypted = cipher.decrypt(cookie)
        unpadded = unpad(decrypted, 16)
    except ValueError as e:
        return {"error": str(e)}

    print(unpadded.split(b";"))
    if b"admin=True" in unpadded.split(b";"):
        return {"flag": FLAG}
    else:
        return {"error": "Only admin can read the flag"}


def get_cookie():
    expires_at = (datetime.today() + timedelta(days=1)).strftime("%s")
    cookie = f"admin=False;expiry={expires_at}".encode()

    iv = os.urandom(16)
    padded = pad(cookie, 16)
    cipher = AES.new(KEY, AES.MODE_CBC, iv)
    encrypted = cipher.encrypt(padded)
    ciphertext = iv.hex() + encrypted.hex()

    return {"cookie": ciphertext}

cookie = get_cookie()["cookie"]
print(f"{cookie=}")

iv = bytearray.fromhex(cookie[:32])

admin_false = "admin=False;expi".encode()
admin_true = ";admin=True;expi".encode()


x = xor(iv, admin_false)
new_iv = xor(x, admin_true)
new_cookie = new_iv.hex() + cookie[32:]

print(new_iv.hex())
print(new_cookie)

print(check_admin(new_cookie, new_iv.hex()))
