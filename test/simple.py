import matplotlib.pyplot as plt
import numpy
import snn

numpy.random.seed(0)

N = 1000
Ne = N * 0.8
Ni = N - Ne

network = snn.network(N)

v = -65 * numpy.ones((N, 1))
u = numpy.multiply(network.b, v)

firings = [[], []]

for t in range(1000):
    network.I = 13 * (numpy.random.rand(N, 1) - 0.5)
    fired = numpy.argwhere(v >= 30)[:,0]
    if fired.size > 0:
        for firing in fired:
            firings[0].append(t)
            firings[1].append(firing)
        v[fired] = network.c[fired]
        u[fired] += network.d[fired]
        network.I += numpy.sum(network.s[:, fired], 1).reshape((N, 1))
    v = v + (0.04 * numpy.square(v) + 5 * v + 140 - u + network.I)
    u = u + numpy.multiply(network.a, numpy.multiply(network.b, v) - u)

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
