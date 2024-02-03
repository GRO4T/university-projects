from pwn import xor


cookie = "845dd814c4b3777ae66614f19d914a2ad2af7c96cb3744b24053f4b695b157555fde320269f8556e845afe86a30b6076"
iv = bytearray.fromhex(cookie[:32])

admin_false = "admin=False;".encode()
admin_true = ";admin=True;".encode()


x = xor(iv, admin_false)
new_iv = xor(x, admin_true)
new_cookie = new_iv.hex() + cookie[32:]

print(new_iv.hex())
print(new_cookie)

