import matplotlib.pyplot as plt
import numpy
import snn
import pdb

numpy.random.seed(0)

N = 1000

network = snn.network(N)

firings = [[], []]

for t in range(1000):
    network.I[:] = 13 * (numpy.random.rand(N, 1) - 0.5)
    network.process_firings()
    network.update_potentials()
    if network.fired.size > 0:
        for firing in network.fired:
            firings[0].append(t)
            firings[1].append(firing)

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
