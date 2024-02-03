import numpy as np
import benchmark_functions as bf



hypersphere = bf.Hypersphere(n_dimensions=2)
hypersphere_v = np.frompyfunc(lambda x,y: hypersphere((x, y)), 2, 1)

easom = bf.Easom()
easom_v = np.frompyfunc(lambda x,y: easom((x, y)), 2, 1)

himmelblau = bf.Himmelblau()
himmelblau_v = np.frompyfunc(lambda x,y: himmelblau((x, y)), 2, 1)

schwefel = bf.Schwefel(n_dimensions=2)
schwefel_v = np.frompyfunc(lambda x,y: schwefel((x, y)), 2, 1)
