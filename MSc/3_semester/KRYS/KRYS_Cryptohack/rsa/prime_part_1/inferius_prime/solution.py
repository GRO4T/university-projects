from Crypto.Util.number import getPrime, inverse, bytes_to_long, long_to_bytes, GCD

n = 742449129124467073921545687640895127535705902454369756401331
e = 3
ct = 39207274348578481322317340648475596807303160111338236677373

phi = (752708788837165590355094155871-1) * (986369682585281993933185289261-1)

print(f"n = {n}")
print(f"e = {e}")
print(f"ct = {ct}")
d = inverse(e, phi)

pt = pow(ct, d, n)
decrypted = long_to_bytes(pt).decode()
print(f"{decrypted=}")


