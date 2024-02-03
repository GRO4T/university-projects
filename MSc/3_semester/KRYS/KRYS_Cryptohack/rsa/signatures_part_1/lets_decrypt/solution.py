from pwn import * # pip install pwntools
import json
from Crypto.Util.number import bytes_to_long, long_to_bytes
import re
from pkcs1 import emsa_pkcs1_v15


n = 0x80a5b8245992f64d2d11f1279a88d23dd76594fa0a37427e55482c819fe6eb727f7cde0a715c018f8e3c98cc721ff1f779e8c704f3ad2ce877eaecf680c7105685581ac1dc8d5040d602a47d8dba333e787599a7528154f8f72581f1119c1c1267078e8bfb4e2a67b00ad13827660381b8105590a72b7d1fa45bdd2c07cc12ab2d8f7c6578178e93aac1a29ccdd1ecbeeafa25fa425988376384f745a7613af437ad235826d7d1499a0b2e83401ce0781fb85aa801ae9c43a9f79b2bd4bc8817a2a0725272e847e3b397e1d27b86fa0ba038b2704534a98a12e5e133f2358521ee0ee59a892928543c4a71460cadc2c89e011b019c2094ca6d17a0c143dc9297
e = 0x10001
signature = 0x55c231eebc642cd1e44199e10937ee8b9e93c0c2d10a18b7b53a207fb1ddd4e6c2e08368a1943187bb1efe0378567340a0851710c426f609aa79d3b5bb3f8efe7f531cfdb54a9fba9e77e3ca2adcecdc299ebf601bd8926dd6ed4e7e71f96ef61cc041159eb0584ff4ce9f0d9e5cb49a91ba15226740f378340e40805aff2e20e275b783aa43a0ac670ec1af2d4e834acceda189add6ed7daf64ed8f9f9718f030c8a7d64afee7cf33beef5f790611eaef40e7c978e2355f3039a6df4f38113ce83ed669a733ce6a93e1fb04fdd6c28815beb6b62f886a47150fbdd34668aa7ff55787874a7b6787a5942da4d73b3197eb792b39d0e338f48fc5f4c01a16a178

calculated_digest = pow(signature, e, n)
print(calculated_digest)

msg = "I am Malloryown CryptoHack.org"
digest = bytes_to_long(emsa_pkcs1_v15.encode(msg.encode(), 256))

print(f"{calculated_digest=}")
print(f"{digest=}")

print("----------")
print(signature-digest)
print('------------')
print(pow(signature, 1, signature-digest))


new_n = hex(signature-digest)
print(f"{new_n=}")


# r = remote('socket.cryptohack.org', 13391, level = 'debug')

# def json_recv():
#     line = r.recvline()
#     return json.loads(line.decode())

# def json_send(hsh):
#     request = json.dumps(hsh).encode()
#     r.sendline(request)

# r.recvline()
# json_send({"option":"get_signature"})
# res = json_recv()

# req = res
# req["option"] = "verify"
# req["msg"] = res["signature"]
# req.pop("signature")

# json_send(req)

# json_recv()
