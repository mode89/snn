import matplotlib.pyplot as plt
import numpy

N = 1000
Ne = N * 0.8
Ni = N - Ne

re = numpy.random.rand(Ne)
ri = numpy.random.rand(Ni)

a = numpy.concatenate((
    0.02 * numpy.ones(Ne),
    0.02 + 0.08 * ri
))
b = numpy.concatenate((
    0.2 * numpy.ones(Ne),
    0.25 - 0.05 * ri
))
c = numpy.concatenate((
    -65 + 15 * numpy.square(re),
    -65 * numpy.ones(Ni)
))
d = numpy.concatenate((
    8 - 6 * numpy.square(re),
    2 * numpy.ones(Ni)
))

S = numpy.concatenate((
    0.5 * numpy.random.rand(N, Ne),
    -1.0 * numpy.random.rand(N, Ni)), axis=1)

v = -65 * numpy.ones(N)
u = numpy.multiply(b, v)

firings = [[], []]

for t in range(1000):
    I = numpy.concatenate((
        5 * numpy.random.randn(Ne),
        2 * numpy.random.randn(Ni)
    ))
    fired = numpy.argwhere(v >= 30)
    if fired.size > 0:
        for firing in fired:
            firings[0].append(t)
            firings[1].append(firing)
        v[fired] = c[fired]
        u[fired] += d[fired]
        I += numpy.sum(S[:, numpy.transpose(fired)[0]], 1)
    v = v + (0.04 * numpy.square(v) + 5 * v + 140 - u + I)
    u = u + numpy.multiply(a, numpy.multiply(b, v) - u)

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
