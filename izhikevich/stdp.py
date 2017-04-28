import matplotlib.pyplot as plt
import numpy
import random

random.seed(0)

T = 1000
N = 1000
Ne = int(N * 0.8)
Ni = N - Ne

M = int(N * 0.1)
D = 20

a = numpy.concatenate((
    0.02 * numpy.ones(Ne),
    0.1 * numpy.ones(Ni)
))
d = numpy.concatenate((
    8 * numpy.ones(Ne),
    2 * numpy.ones(Ni)
))

post = numpy.empty((N, M), dtype=numpy.int)
for i in range(Ne):
    post[i,:] = random.sample(range(N), M)
for i in range(Ne, N):
    post[i,:] = random.sample(range(Ne), M)

delays = [[[] for i in range(D)] for j in range(N)]
for i in range(Ne):
    for j in range(M):
        delays[i][int(D * random.random())].append(j)
for i in range(Ne, N):
    delays[i][0] = range(M)

s = numpy.concatenate((
    6 * numpy.ones((Ne, M)),
    -5 * numpy.ones((Ni, M))
))

v = -65 * numpy.ones(N)
u = 0.2 * v

firings = []

for t in range(T):
    print(t)

    I = numpy.zeros(N)
    I[int(N * random.random())] = 20.0

    fired = numpy.argwhere(v >= 30)
    if fired.size > 0:
        v[fired] = -65.0
        u[fired] = u[fired] + d[fired]

    firings.append(fired)
    for time in range(min(D, len(firings))):
        for fired_i in firings[t - time]:
            synapses = delays[fired_i][time]
            if len(synapses) > 0:
                ind = post[fired_i, synapses]
                I[ind] += s[fired_i, synapses]

    for i in range(2):
        v += 0.5 * ((0.04 * v + 5.0) * v + 140.0 - u + I)
    u += a * (0.2 * v - u)

x = []
y = []
for t in range(T):
    for fired in firings[t]:
        x.append(t)
        y.append(fired)

plt.scatter(
    x, y,
    color="black",
    marker=".")
plt.show()
