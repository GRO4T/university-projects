import os
from Crypto.Cipher import AES


KEY = os.urandom(16)
FLAG = "crypto{" + "x" * 24 + "}"


def decrypt(ciphertext):
    ciphertext = bytes.fromhex(ciphertext)

    cipher = AES.new(KEY, AES.MODE_ECB)
    try:
        decrypted = cipher.decrypt(ciphertext)
    except ValueError as e:
        return {"error": str(e)}

    return {"plaintext": decrypted.hex()}


def encrypt_flag():
    iv = os.urandom(16)

    cipher = AES.new(KEY, AES.MODE_CBC, iv)
    encrypted = cipher.encrypt(FLAG.encode())
    ciphertext = iv.hex() + encrypted.hex()

    return {"ciphertext": ciphertext}


def print_by_block(hex):
    for i in range(3):
        print(hex[32*i:32*i+32])

print("--------------------")

r = encrypt_flag()
print_by_block(r["ciphertext"])

print("--------------------")

r2 = decrypt(r["ciphertext"])
print_by_block(r2["plaintext"])

print("==========================")
encrypted = "399adb6d86a44abc223823c1fb181e5b11607b853b056d08b20bfcaa00ad96c224eb0e6a16ae0f23ee2898b172970c1f"
print_by_block(y)
print("==========================")
decrypted = "e54498604666a34aa3c0590cb97fc7d85ae8a21df2cb318f415a7cf48e7b756e4e540db50a6132398554dd8b218cb7bf"
print_by_block(x)

# crypto{3cb_5uck5_4v01d_17_!!!!!}
x = "28faa0e8bda127b49033df6bfbb58899" # first block xor 