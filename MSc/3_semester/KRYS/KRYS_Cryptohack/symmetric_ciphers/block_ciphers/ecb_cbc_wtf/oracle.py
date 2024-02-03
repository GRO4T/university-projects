import requests


URL = "https://aes.cryptohack.org/ecbcbcwtf/"

CRYPTO_HEX = "63727970746f7b"

while True:
    r  = requests.get(URL + "encrypt_flag/")
    c = r.json()["ciphertext"]
    r2 = requests.get(URL + "decrypt/" + c)
    p = r2.json()["plaintext"]
    if CRYPTO_HEX in p:
        print(p)
