from Crypto.Util.number import bytes_to_long, long_to_bytes, inverse
from pwn import *
import json

r = remote("socket.cryptohack.org",13376, level="debug")

n = 0x954e1412ba207b8a246ea515e81425aeb5471cf5062b6497b2c76312ccf150498779ca540464b09fe573df68b0cfdcac124ba799b8546b45b49eaae9fadd630d1b5562a9993c6a3da72d5222e24aa6e1f9c663bfd07f31f0cdef87a54f2fbf7151afc3fd329bd16692dcfa6794c3d94d00fb2e11b49557a491be3e510f0c3e22163487df65e54d68f43a3ecea44e48dc929f2d321c6bfdb2c6c233c704e0618041ace0be91f637f423e6161b36a1fe0f04445ee1f48dc5960659706bbcb97c1667c5f17d0f2395dad348a88f3efb7fa06f99f7963749679eb697cd178fce6f65cfee5b6c9c36096c96f5b5532a6a3b44127afe27f10015dd71a644d455f800d5
e = 0x10001

def json_recv():
    line = r.recvline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)

msg = b"admin=True"
msg_i = bytes_to_long(msg)

r.recvline()

json_send({"option": "sign", "msg": "02"})
res = json_recv()

sig_2 = int(res["signature"][2:],16)

msg_i_2 = msg_i*2

json_send({"option": "sign", "msg": hex(msg_i_2)[2:]})
res = json_recv()

sig_msg_2 = int(res["signature"][2:],16)

sig = (inverse(sig_2, n) * sig_msg_2) % n

json_send({"option": "verify", "msg": hex(msg_i)[2:], "signature": hex(sig)[2:]})
res = json_recv()