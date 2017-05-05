import matplotlib.pyplot as plt
import numpy
import snn

numpy.random.seed(0)

N = 1000
Ne = N * 0.8
Ni = N - Ne

network = snn.network(N)

firings = [[], []]

for t in range(1000):
    network.I[:] = 13 * (numpy.random.rand(N, 1) - 0.5)
    fired = numpy.argwhere(network.v >= 30)[:,0]
    if fired.size > 0:
        for firing in fired:
            firings[0].append(t)
            firings[1].append(firing)
        network.v[fired] = network.c[fired]
        network.u[fired] += network.d[fired]
        network.I += numpy.sum(network.s[:, fired], 1).reshape((N, 1))
    network.update_potentials()

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
