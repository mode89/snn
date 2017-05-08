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
    0.02 * numpy.ones((Ne, 1)),
    0.1 * numpy.ones((Ni, 1))
))
d = numpy.concatenate((
    8 * numpy.ones((Ne, 1)),
    2 * numpy.ones((Ni, 1))
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

v = -65 * numpy.ones((N, 1))
u = 0.2 * v

firings = numpy.empty((0, 2), dtype=numpy.int)

for t in range(T):
    print(t)

    I = numpy.zeros((N, 1))
    I[int(N * random.random())] = 20.0

    fired = numpy.argwhere(v >= 30)[:,0]
    fired = fired.reshape((len(fired), 1))

    if fired.size > 0:
        firings = numpy.vstack((
            firings,
            numpy.hstack((
                t * numpy.ones_like(fired),
                fired
            ))
        ))

        v[fired] = -65.0
        u[fired] = u[fired] + d[fired]

    k = firings.shape[0] - 1
    while k >= 0 and firings[k, 0] > (t - D):
        time, f_index = firings[k]
        synapses = delays[f_index][t - time]
        if len(synapses) > 0:
            ind = post[f_index, synapses]
            I[ind] += s[f_index, synapses].reshape((len(ind), 1))
        k -= 1

    for i in range(2):
        v += 0.5 * ((0.04 * v + 5.0) * v + 140.0 - u + I)
    u += a * (0.2 * v - u)

plt.scatter(
    firings[:,0],
    firings[:,1],
    color="black",
    marker=".")
plt.show()
