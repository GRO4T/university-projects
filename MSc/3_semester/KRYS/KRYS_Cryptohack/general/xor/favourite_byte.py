from pwn import xor

data = bytearray.fromhex("73626960647f6b206821204f21254f7d694f7624662065622127234f726927756d")

for i in range(256):
    byte = i.to_bytes()
    print(xor(data, byte))