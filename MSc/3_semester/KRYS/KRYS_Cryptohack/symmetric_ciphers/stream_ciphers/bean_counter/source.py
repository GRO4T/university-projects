import requests

png_header = bytes([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52])
# png_header = "89504e470d0a1a0a0000000d49484452"

url = "http://aes.cryptohack.org/bean_counter/encrypt/"
r = requests.get(url)
e = bytearray.fromhex(r.json()["encrypted"])

keystream = []
for i in range(len(png_header)):
    keystream.append(png_header[i] ^ e[i])

png = [0] * len(e)

for i in range(len(e)):
    png[i] = e[i] ^ keystream[i % len(keystream)]

with open("bean_flag.png", "wb") as f:
    f.write(bytes(png))