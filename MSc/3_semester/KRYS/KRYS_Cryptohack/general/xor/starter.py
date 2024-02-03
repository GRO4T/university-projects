from pwn import xor

msg = "label"

result = xor(msg, 13).decode()

print(result)