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

# generate post-synaptic connections
post = numpy.empty((N, M), dtype=numpy.int)
for i in range(Ne):
    post[i,:] = random.sample(range(N), M)
for i in range(Ne, N):
    post[i,:] = random.sample(range(Ne), M)

# find pre-synaptic connections to excitatory neurons
pre = [[] for i in range(N)]
for i in range(Ne):
    for j in range(M):
        pre[post[i,j]].append(i)

# generate delays
delays = [[[] for i in range(D)] for j in range(N)]
for i in range(Ne):
    for j in range(M):
        delays[i][int(D * random.random())].append(post[i,j])
for i in range(Ne, N):
    for j in range(M):
        delays[i][0].append(post[i,j])

# generate matrix of synaptic weights
s = numpy.zeros((N, N))
for i in range(Ne):
    s[i, post[i,:]] = 6.0
for i in range(Ne, N):
    s[i, post[i,:]] = -5.0

v = -65 * numpy.ones(N)
u = 0.2 * v

firings = []

STDP = numpy.zeros(N)

for t in range(T):
    print(t)

    # provide random input
    I = numpy.zeros(N)
    I[int(N * random.random())] = 20.0

    # identify fired neurons
    fired = numpy.argwhere(v >= 30)
    if fired.size > 0:
        v[fired] = -65.0
        u[fired] = u[fired] + d[fired]

    # deliver spikes to post-synaptic neurons
    firings.append(fired)
    for time in range(min(D, len(firings))):
        for fired_neuron in firings[t - time]:
            post_neurons = delays[fired_neuron][time]
            if len(post_neurons) > 0:
                I[post_neurons] += s[fired_neuron, post_neurons]

    # update post-synaptic potential
    for i in range(2):
        v += 0.5 * ((0.04 * v + 5.0) * v + 140.0 - u + I)
    u += a * (0.2 * v - u)

    # update synaptic weights
    STDP[fired] = 1.0
    for fired_neuron in fired:
        if fired_neuron < Ne:
            post_neurons = post[fired_neuron,:]
            s[fired_neuron, post_neurons] = numpy.maximum(0.0,
                s[fired_neuron, post_neurons] - 1.2 * STDP[post_neurons])
        pre_neurons = pre[fired_neuron]
        s[pre_neurons, fired_neuron] = numpy.minimum(10.0,
            s[pre_neurons, fired_neuron] + STDP[pre_neurons])
    STDP *= 0.95

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
