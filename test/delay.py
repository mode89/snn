import collections
import matplotlib.pyplot as plt
import numpy
import random
import snn

random.seed(0)

T = 1000
N = 1000
Ne = int(N * 0.8)
Ni = N - Ne

M = int(N * 0.1)
D = 20

network = snn.network(N)

post = numpy.empty((N, M), dtype=numpy.int)
for i in range(Ne):
    post[i,:] = random.sample(range(N), M)
for i in range(Ne, N):
    post[i,:] = random.sample(range(Ne), M)

delays = [[[] for i in range(D)] for j in range(N)]
for i in range(Ne):
    for j in range(M):
        delays[i][int(D * random.random())].append(post[i,j])
for i in range(Ne, N):
    for j in range(M):
        delays[i][0].append(post[i,j])

s = numpy.zeros((N, N))
for i in range(Ne):
    s[i, post[i,:]] = 6.0
for i in range(Ne, N):
    s[i, post[i,:]] = -5.0

firings_hist = []
firings = collections.deque(maxlen=D)

for t in range(T):
    print(t)

    network.I[:] = numpy.zeros((N, 1))
    network.I[int(N * random.random())] = 20.0

    network.find_fired_neurons()
    fired = numpy.copy(network.fired)
    network.reset_fired_neurons()

    firings.appendleft(fired)
    for time in range(len(firings)):
        for fired_neuron in firings[time]:
            post_neurons = delays[fired_neuron][time]
            if len(post_neurons) > 0:
                network.I[post_neurons] += \
                    s[fired_neuron, post_neurons].reshape(
                        (len(post_neurons), 1))

    network.update_potentials()

    if fired.size > 0:
        firings_hist.append((t, fired))

x = []
y = []
for p in firings_hist:
    for fired in p[1]:
        x.append(p[0])
        y.append(fired)

plt.scatter(
    x, y,
    color="black",
    marker=".")
plt.show()
