import matplotlib.pyplot as plt
import numpy
import random

random.seed(0)

T = 100000
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
sm = 4.0
sd = numpy.zeros((N, N))

DA = 0
rew = []
n1 = 700
n2 = post[n1, 0]
s[n1, n2] = 0.0

interval = 20
n1f = []
n2f = []

class data:
    n0 = 700
    n1 = post[n0, 0]
    stdp0 = []
    stdp1 = []
    s01 = []
    da = []

for t in range(T):
    print(t)

    # provide random input
    I = 13.0 * numpy.array([random.uniform(-0.5, 0.5) for i in range(N)])

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
    STDP[fired] = 0.1
    for fired_neuron in fired:
        if fired_neuron < Ne:
            post_neurons = post[fired_neuron,:]
            sd[fired_neuron, post_neurons] -= 1.2 * STDP[post_neurons]
        pre_neurons = pre[fired_neuron]
        sd[pre_neurons, fired_neuron] += STDP[pre_neurons]
    STDP *= 0.95

    DA *= 0.995
    if t % 10 == 0:
        s[0:Ne,:] = numpy.maximum(0.0, numpy.minimum(sm,
            s[0:Ne,:] + (0.002 + DA) * sd[0:Ne,:]))
        sd *= 0.99
    if numpy.any(fired == n1):
        n1f.append(t)
    if numpy.any(fired == n2):
        n2f.append(t)
        if len(n1f) > 0:
            if t - n1f[-1] < interval and n2f[-1] > n1f[-1]:
                print("Coincident spiking")
                rew.append(t + 1000 + int(2000 * random.random()))
    if any([it == t for it in rew]):
        print("Rewarding")
        DA += 0.5

    data.stdp0.append(STDP[data.n0])
    data.stdp1.append(STDP[data.n1])
    data.s01.append(s[data.n0, data.n1])
    data.da.append(DA)

x = []
y = []
for t in range(T):
    for fired in firings[t]:
        x.append(t)
        y.append(fired)

plt.subplot(411)
plt.scatter(x, y, color="black", marker=".")
plt.xlim(0, T)
plt.ylim(0, N)

plt.subplot(412)
plt.plot(
    range(T), data.stdp0,
    range(T), data.stdp1)
plt.xlim(0, T)

plt.subplot(413)
plt.plot(range(T), data.s01)
plt.xlim(0, T)

plt.subplot(414)
plt.plot(range(T), data.da)
plt.xlim(0, T)

plt.show()
