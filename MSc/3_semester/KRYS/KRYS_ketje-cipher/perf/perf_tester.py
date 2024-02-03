import string
import random
import subprocess
import time
import matplotlib.pyplot as plt

# Ketje configuration
KEY_SIZE = 16
NONCE_SIZE = 4

TEST_SPEC = {
    "KetjeJr": {
        "KRYS": {
            "color": "green",
            "symbol": "^",
        },
        "KeccakTools": {
            "color": "gold",
            "symbol": "s",
        }
    },
    "KetjeSr": {
        "KRYS": {
            "color": "blue",
            "symbol": "^",
        },
        "KeccakTools": {
            "color": "red",
            "symbol": "s",
        }
    },
    "KetjeMinor": {
        "KRYS": {
            "color": "cyan",
            "symbol": "^",
        },
        "KeccakTools": {
            "color": "coral",
            "symbol": "s",
        }
    },
    "KetjeMajor": {
        "KRYS": {
            "color": "purple",
            "symbol": "^",
        },
        "KeccakTools": {
            "color": "pink",
            "symbol": "s",
        }
    },
}

# Test configuration
N_RUNS = 5
MESSAGE_SIZE_LIST = [12, 36, 60, 84, 108]
MESSAGE_SIZE_MULTIPLIER = 10
MESSAGE_SIZE_LIST = [MESSAGE_SIZE_MULTIPLIER * D for D in MESSAGE_SIZE_LIST]

results = {}
for ketje_flavour in TEST_SPEC.keys():
    results[ketje_flavour] = {
        "KRYS": [0] * len(MESSAGE_SIZE_LIST),
        "KeccakTools": [0] * len(MESSAGE_SIZE_LIST)
    }

for ketje_flavour in TEST_SPEC.keys():
    for run in range(N_RUNS):
        for idx, D in enumerate(MESSAGE_SIZE_LIST):
            ketje_binary = f"../build/{ketje_flavour}"
            plaintext = "".join(random.choice(string.ascii_letters) for _ in range(D))
            nonce = "".join(random.choice(string.ascii_letters) for _ in range(NONCE_SIZE))
            key = "".join(random.choice(string.ascii_letters) for _ in range(KEY_SIZE))
            associated_data = "".join(random.choice(string.ascii_letters) for _ in range(D))
            options = f"-p {plaintext} -n {nonce} -k {key} -a {associated_data}".split()

            # Test KRYS implementation
            start = time.time()
            subprocess.run([ketje_binary, "encrypt", "-i", "KRYS", *options], stdout=subprocess.DEVNULL)
            end = time.time()
            exec_time_ms = (end - start) * 1000
            print(f"(KRYS) {D=}: {exec_time_ms}ms")
            results[ketje_flavour]["KRYS"][idx] += exec_time_ms

            # Test KeccakTools implementation
            start = time.time()
            subprocess.run([ketje_binary, "encrypt", "-i", "KeccakTools", *options], stdout=subprocess.DEVNULL)
            end = time.time()
            exec_time_ms = (end - start) * 1000
            print(f"(KeccakTools) {D=}: {exec_time_ms}ms")
            results[ketje_flavour]["KeccakTools"][idx] += exec_time_ms

# Graphs
plt.title("Porównanie implementacji Ketje (szyfrowanie)")

for ketje_flavour in TEST_SPEC.keys():
    krys_results = results[ketje_flavour]["KRYS"]
    keccak_tools_results = results[ketje_flavour]["KeccakTools"]
    krys_results = [res / N_RUNS for res in krys_results]
    keccak_tools_results = [res / N_RUNS for res in keccak_tools_results]
    plt.plot(MESSAGE_SIZE_LIST, krys_results, TEST_SPEC[ketje_flavour]["KRYS"]["symbol"], color=TEST_SPEC[ketje_flavour]["KRYS"]["color"], label=f"{ketje_flavour} (KRYS)")
    plt.plot(MESSAGE_SIZE_LIST, krys_results, "-", color=TEST_SPEC[ketje_flavour]["KRYS"]["color"])
    plt.plot(MESSAGE_SIZE_LIST, keccak_tools_results, TEST_SPEC[ketje_flavour]["KeccakTools"]["symbol"], color=TEST_SPEC[ketje_flavour]["KeccakTools"]["color"], label=f"{ketje_flavour} (KeccakTools)")
    plt.plot(MESSAGE_SIZE_LIST, keccak_tools_results, "--", color=TEST_SPEC[ketje_flavour]["KeccakTools"]["color"])
plt.ylabel("Czas (ms)")
plt.xlabel("Rozmiar wiadomości (w bajtach)")
plt.legend()
plt.show()
