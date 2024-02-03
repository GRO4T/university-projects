import sys
import matplotlib.pyplot as plt

algo = sys.argv[1]
func_id = sys.argv[2]

titles = {
    "random": "Proste przeszukiwanie losowe dla funkcji z zadania ",
    "swarm": "Optymalizacja rojem cząstek dla funkcji z zadania "
}

threads = list(range(1, 17))

times = []

for line in sys.stdin:
    exec_time = line.strip().replace(',', '')
    times.append(float(exec_time))


plt.plot(threads, times, 'o')
plt.xlabel("liczba wątków")
plt.ylabel("czas w sekundach")
plt.title(titles[algo] + str(func_id) + ".")
plt.savefig(f"../doc/test_parallelism_{algo}_f{func_id}.png")