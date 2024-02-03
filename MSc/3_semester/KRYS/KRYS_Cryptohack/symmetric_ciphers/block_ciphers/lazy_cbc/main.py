import requests

URL = "https://aes.cryptohack.org/lazy_cbc"


plain = (b'a'*16*3).hex()
print(plain)

ct = "740a9bbd9d7db10019102d781af464bbc0aaf638f2b7ecc5825af1d4bd67aaadbaa6584cdba1e12172a1062d86bf2810"
fake_ct = ct[:32] + '0'*32 + ct[:32]
print(fake_ct)

fake_plain = "616161616161616161616161616161612025a3ad7aca06b05cccafbe2aae1da0eefdaacdf5c95aa4568da795b9725700"
fake_plain = bytearray.fromhex(fake_plain)
key = [0]*16
for i in range(len(key)):
   key[i] = fake_plain[i] ^ fake_plain[32+i]

key = bytearray(key)
print(key.hex())

