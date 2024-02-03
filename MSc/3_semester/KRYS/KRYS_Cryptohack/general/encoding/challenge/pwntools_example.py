from pwn import * # pip install pwntools
import json

r = remote('socket.cryptohack.org', 13377, level = 'debug')

def json_recv():
    line = r.recvline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)

for i in range(100):
    print(f"[PROBLEM{i+1}]")
    received = json_recv()

    print("Received type: ")
    print(received["type"])
    print("Received encoded value: ")
    print(received["encoded"])

    msg_type = received["type"]
    msg_encoded = received["encoded"]

    msg_decoded = ""
    match msg_type:
        case "base64":
            import base64
            msg_decoded = base64.b64decode(msg_encoded).decode()
        case "utf-8":
            msg_decoded = "".join(map(lambda x: chr(x), msg_encoded))
        case "hex":
            msg_decoded = bytearray.fromhex(msg_encoded).decode()
        case "rot13":
            import codecs
            msg_decoded = codecs.decode(msg_encoded, "rot_13")
        case "bigint":
            msg_decoded = bytearray.fromhex(msg_encoded[2:]).decode()

    to_send = {
        "decoded": msg_decoded
    }
    json_send(to_send)

print("Getting this flag")
received = json_recv()
print(received)