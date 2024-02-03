import argparse
import time
import logging
import datetime
import matplotlib.pyplot as plt
import numpy as np

from particle_swarm import ParticleSwarm
from test_functions import hypersphere_v, easom_v, himmelblau_v, schwefel_v
from logger import configure_logging


logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class ParticleSwarmRunner:
	obj_functions = {
		"schwefel":  schwefel_v,
		"easom": easom_v,
		"himmelblau": himmelblau_v,
		"hypersphere": hypersphere_v
	}

	omega_policies = {
		"basic": lambda p: None,
		"random": lambda p: p.set_update_omega_randomly(),
		"iteration": lambda p: p.set_update_omega_iteration(),
		"global_minimum": lambda p: p.set_update_omega_global_minimum(),
	}

	stop_conditions = {
		"max_iteration": lambda p: p.stop_condition_max_iteration(),
	}

	def __init__(self):
		# np.random.seed(int(time.time()))
		np.random.seed(100)
		# np.random.seed(200)
		# np.random.seed(300)
		self.parser = argparse.ArgumentParser(description="Runner for particle swarm algorithm")
		self.parser.add_argument("--particles", type=int, help="Number of particles", required=True)
		self.parser.add_argument("--obj_func", type=str, help="Objective function", required=True)
		self.parser.add_argument("--stop_cond", type=str, help="Stop condition")
		self.parser.add_argument("--omega_policy", type=str, help="Omega update policy", required=True)
		self.parser.add_argument("--max_iteration", type=int, help="Maximum number of iterations")
		self.parser.add_argument("--limit", type=int, help="The distance from the point (0, 0) at which the particles can appear at start", default=5)
		self.parser.add_argument("--omega", type=float, help="Value of omega", default=0.8)

		self.parser.add_argument("--c_local", type=float, help="Value of omega", default=0.1)
		self.parser.add_argument("--c_global", type=float, help="Value of omega", default=0.1)


		self.parser.add_argument("--test", help="Perform test (multiple runs) and show summary", action="store_true")
		self.parser.add_argument("--tests", type=int, help="Number of tests", default=5)

		self.parser.add_argument("--graph", help="Generate execution graph of a single run", action="store_true")
		self.parser.add_argument("--graph_omega", help="Generate omega graph of a single run", action="store_true")

		self.parser.add_argument("--gif", help="Generate a GIF of a single run. Additional values required xlim and ylim, e.g. --gif 750 750", nargs=2)
		self.parser.add_argument("--frames", type=int, help="Number of frames to generate")

		self.args = self.parser.parse_args()
		mode = None
		if self.args.test:
			mode = "test"
		elif self.args.graph:
			mode = "graph"
		elif self.args.gif:
			mode = "animate"
		else:
			raise Exception("Mode not specified!")
		self.logfile = f"logs/{mode}_{self.filename_base}.log"
		configure_logging(self.logfile)
		print(f"Logs in: {self.logfile}")

	@property
	def filename_base(self) -> str:
		curr_time = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
		return f"{self.obj_func}_{self.omega_policy}_{self.stop_cond}_{self.particles}.{curr_time}"

	@property
	def particles(self) -> int:
		return self.args.particles

	@property
	def obj_func(self) -> str:
		return self.args.obj_func.lower()

	@property
	def omega_policy(self) -> str:
		return self.args.omega_policy.lower()

	@property
	def stop_cond(self) -> str:
		if self.args.stop_cond:
			return self.args.stop_cond.lower()
		return ""

	@property
	def xlim(self) -> int:
		return int(self.args.gif[0])

	@property
	def ylim(self) -> int:
		return int(self.args.gif[1])

	@property
	def limit(self) -> int:
		return int(self.args.limit)

	def run(self):
		if self.args.test:
			self.test()
		elif self.args.graph:
			self.graph()
		elif self.args.gif:
			self.animate()
		else:
			logger.warning("Flags not provided. Did nothing.")

	def test(self):
		logger.info(f"[TEST] {self.obj_func.upper()} (omega_policy: {self.omega_policy} stop_cond: {self.stop_cond})")
		iterations = list()
		global_mins = list()
		start = time.perf_counter()
		for i in range(self.args.tests):
			p = ParticleSwarm(n_particles=self.particles, c_local=self.args.c_local, c_global=self.args.c_global,
						  		objective_func=ParticleSwarmRunner.obj_functions[self.obj_func],
						  		max_iteration=self.args.max_iteration, limit=self.limit, w=self.args.omega)
			ParticleSwarmRunner.omega_policies[self.omega_policy](p)
			while ParticleSwarmRunner.stop_conditions[self.stop_cond](p):
				p.update()
			iterations.append(p.iteration)
			global_mins.append(p.g_best_vals)
		stop = time.perf_counter()
		self.summary(global_mins, iterations, stop, start)

	def animate(self):
		logger.info(f"[ANIMATE] {self.obj_func.upper()} (omega_policy: {self.omega_policy} stop_cond: {self.stop_cond})")
		p = ParticleSwarm(self.particles, ParticleSwarmRunner.obj_functions[self.obj_func], 
						  limit=self.limit, w=self.args.omega, c_local=self.args.c_local, c_global=self.args.c_global)
		p.set_animation_params(self.xlim, self.ylim)
		p.prepare_animation()
		ParticleSwarmRunner.omega_policies[self.omega_policy](p)
		path = f"gifs/{self.filename_base}.gif"
		p.animation(path, self.args.frames)
		print(f"GIF in: {path}")

	def graph(self):
		global_mins = []
		omegas = []
		logger.info(f"[GRAPH] {self.obj_func.upper()} (omega_policy: {self.omega_policy} stop_cond: {self.stop_cond})")
		p = ParticleSwarm(n_particles=self.particles, c_local=self.args.c_local, c_global=self.args.c_global,
						  	objective_func=ParticleSwarmRunner.obj_functions[self.obj_func],
						  	limit=self.limit, max_iteration=self.args.max_iteration, w=self.args.omega)
		ParticleSwarmRunner.omega_policies[self.omega_policy](p)
		while ParticleSwarmRunner.stop_conditions[self.stop_cond](p):
			p.update()
			global_mins.append(p.g_best_vals)
			omegas.append(p.w)

		x = range(p.iteration)
		y = global_mins

		plt.plot(x, y)
		plt.xlabel("Iteracje")
		plt.ylabel("Wartość globalnego minimum")
		plt.title(f"{self.obj_func}(t) {self.omega_policy}")
		path = f"graphs/{self.filename_base}.png"
		plt.savefig(path)
		print(f"Graph in: {path}")
		plt.clf()

		if self.args.graph_omega:
			self.graph_omega(x, omegas)

		logger.debug(f"iterations={p.iteration}")
		logger.debug(f"global_mins={global_mins}")
		logger.debug(f"len(global_mins)={len(global_mins)}")

	def graph_omega(self, x, y):
		logger.debug(f"omegas={y}")

		plt.plot(x, y)
		plt.xlabel("Iteracje")
		plt.ylabel("Wartość omega")
		plt.title(f"omega(t) {self.obj_func} {self.omega_policy}")
		path = f"graphs/omega_{self.filename_base}.png"
		plt.savefig(path)
		print(f"Omega graph in: {path}")

	def summary(self, global_mins, iterations, stop, start):
		logger.info(f"[SUMMARY] {self.obj_func.upper()} (omega_policy: {self.omega_policy} stop_cond: {self.stop_cond})")
		logger.info(f"{'Total time':35}: {stop - start:0.4f} seconds")
		logger.info(f"{'Average global minimum':35}: {sum(global_mins) / len(global_mins)}")
		logger.info(f"{'Minimal global minimum' :35}: {np.amin(global_mins)}")
		logger.info(f"{'Maximal global minimum' :35}: {np.amax(global_mins)}")
		logger.info(f"{'Average time of algorithm':35}: {(stop - start) / self.args.tests:0.4f} seconds")
		plt.hist(global_mins, bins="sqrt")
		plt.title(f"histogram {self.obj_func} {self.omega_policy}")
		path = f"graphs/hist_{self.filename_base}.png"
		plt.savefig(path)
		print(f"Histogram in: {path}")



if __name__ == '__main__':
	ParticleSwarmRunner().run()
