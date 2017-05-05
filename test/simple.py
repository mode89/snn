import matplotlib.pyplot as plt
import numpy

numpy.random.seed(0)

N = 1000
Ne = N * 0.8
Ni = N - Ne

a = numpy.concatenate((
    0.02 * numpy.ones((Ne, 1)),
    0.1 * numpy.ones((Ni, 1))
))
b = numpy.concatenate((
    0.2 * numpy.ones((Ne, 1)),
    0.2 * numpy.ones((Ni, 1))
))
c = numpy.concatenate((
    -65 * numpy.ones((Ne, 1)),
    -65 * numpy.ones((Ni, 1))
))
d = numpy.concatenate((
    8 * numpy.ones((Ne, 1)),
    2 * numpy.ones((Ni, 1))
))

S = numpy.concatenate((
    0.5 * numpy.random.rand(N, Ne),
    -1.0 * numpy.random.rand(N, Ni)), axis=1)

v = -65 * numpy.ones((N, 1))
u = numpy.multiply(b, v)

firings = [[], []]

for t in range(1000):
    I = 13 * (numpy.random.rand(N, 1) - 0.5)
    fired = numpy.argwhere(v >= 30)[:,0]
    if fired.size > 0:
        for firing in fired:
            firings[0].append(t)
            firings[1].append(firing)
        v[fired] = c[fired]
        u[fired] += d[fired]
        I += numpy.sum(S[:, fired], 1).reshape((N, 1))
    v = v + (0.04 * numpy.square(v) + 5 * v + 140 - u + I)
    u = u + numpy.multiply(a, numpy.multiply(b, v) - u)

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
