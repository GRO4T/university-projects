# PORR_particle_swarm_optimization_in_OpenMP_OpenACC_and_OpenMPI
Implementacja optymalizacji rojem cząstek (oraz prostego przeszukiwania
losowego) przy pomocy różnych technologii wspierających równoległe 
wykonanie kodu:
* OpenMP (gałąź master)
* OpenACC (gałąź master-openacc)
* OpenMPI (gałąź master-openmpi)

----
# Kompilacja

Tworzymy folder `build` i przechodzimy do niego

```
mkdir build
cd build
```

Konfigurujemy projekt

```
cmake ..
```

Kompilujemy

```
cmake --build .
```

# Testowanie algorytmu
## runner
```
$ ./runner -h
usage: runner [-h | --help] [-n | --dimension] DIMENSION [-t | --threads] THREADS
[-i | --iterations] ITERATIONS [-s | --search] SEARCH_ALGORITHM [-f | --obj_func] OBJ_FUNC
```

```
$ ./runner 
---------------------------------------
n: 20
threads: 4
iterations: 100000
search_algorithm: random
objective_func_id: 1
---------------------------------------
Search: random
Result:
{
	"value": 74
	"position": [-8.28835, -9.54035, -20.2434, -10.6677, 11.2021, -3.03875, 13.9682, 15.3011, -10.6865, -10.1196, 2.76903, -4.26184, 16.8387, 15.5975, -10.3477, 8.2999, -18.5393, 2.48105, -11.9296, -15.6039, ]
	"exec_time_in_nanos": 105223053
	"exec_time": 0.105223
}
```
```
$ ./runner -n 50 -t 16 -i 10000 -s swarm
---------------------------------------
n: 50
threads: 16
iterations: 10000
search_algorithm: swarm
objective_func_id: 1
---------------------------------------
Search: swarm
Result:
{
	"value": -5.36871e+07
	"position": [-2.3873, -3.4641, 0.00195812, -0.0325867, 0.00396919, 0.358742, 33.9093, 0.290891, 0.0054658, -0.468637, -0.248944, 4.58262, -13.6362, -3.17518, 0.843873, -7.43172, -1.60979, -1.11461, 7.82605, 1.53818, -24.5503, 2.25529, 4.14486, -9.64952, 25.9706, 18.8753, 7.55107, 0.297182, 17.6965, -3.62814, -32.9565, 26.0179, 19.5599, -7.37969, 20.8117, -37.4399, -0.94533, 0.205607, 4.79949, 115547, 29.1306, 1.55907, 1.01852, 4.64064, -10.3429, 1.61431, -35.8724, 11.6137, -0.386012, 2.09328, ]
	"exec_time_in_nanos": 1043979417
	"exec_time": 1.04398
}
```

## plotter
```
$ ./plotter -h
usage: plotter [-h | --help] [-n | --dimension] DIMENSION
[-i | --iterations] ITERATIONS [-s | --search] SEARCH_ALGORITHM [-f | --obj_func] OBJ_FUNC
```

```
$ ./plotter -n 3
---------------------------------------
n: 3
iterations: 1000
search_algorithm: random
objective_func_id: 1
---------------------------------------
```

```
$ ./plotter -n 2
---------------------------------------
n: 2
iterations: 1000
search_algorithm: random
objective_func_id: 1
---------------------------------------
-------------------------------
iteration: 0
best_result: 34.7869
best_position: (-32.8264, 16.5046)
-------------------------------
```
