import sys
import matplotlib.pyplot as plt

algo = sys.argv[1]
func_id = sys.argv[2]
time_limit = sys.argv[3]
repeat_numb = sys.argv[4]

titles = {
    "random": "Proste przeszukiwanie losowe dla funkcji z zadania ",
    "swarm": "Optymalizacja rojem cząstek dla funkcji z zadania "
}

n_dim = ["20", "50", "100", "200"] * int(repeat_numb)

best_vals = []

for line in sys.stdin:
    best_val = line.strip().replace(',', '')
    best_vals.append(float(best_val))

plt.plot(n_dim, best_vals, 'o')
plt.xlabel("Wymiar zadania (n)")
plt.ylabel("Najlepsza znaleziona wartość funkcji")
plt.title(titles[algo] + str(func_id) + " - rozwiązanie po " + str(time_limit) + " s.")
plt.savefig(f"../doc/test_best_after_time_{algo}_f{func_id}_t{time_limit}.png")
