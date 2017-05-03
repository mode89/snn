import collections
import matplotlib.pyplot as plt
import numpy
import random

random.seed(0)

T = 3600 * 10
N = 1000
Ne = int(N * 0.8)
Ni = N - Ne

M = int(N * 0.1)
D = 1

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
    s[i, post[i,:]] = 1.0
for i in range(Ne, N):
    s[i, post[i,:]] = -1.0

v = -65 * numpy.ones(N)
u = 0.2 * v

firings = collections.deque(maxlen=D)

STDP = numpy.zeros(N)
sm = 4.0
sd = numpy.zeros((N, N))

DA = 0
rew = []

S = range(50)
A = range(50, 100)
B = range(100, 150)

s[S, A] = 0.0
s[S, B] = 0.0

class data:
    firings = []
    s_A = [(0, numpy.sum(s[S, A]))]
    s_B = [(0, numpy.sum(s[S, B]))]

plt.subplot(211)
scatter = plt.scatter([], [], color="black", marker=".")

plt.subplot(212)
line_A, line_B, = plt.plot([], [], [], [])

for sec in range(T):

    data.firings = []
    data.stimulus = []

    for ms in range(1000):
        t = sec * 1000 + ms
        print(t)

        # provide random input
        I = 13.0 * numpy.array(
            [random.uniform(-0.5, 0.5) for i in range(N)])

        # issue a stimulus
        if t % 10000 == 0:
            I[S] = 20.0
            count_period = range(t + 1, t + 20)
            count_A = 0
            count_B = 0

        # identify fired neurons
        fired = numpy.argwhere(v >= 30)
        if fired.size > 0:
            v[fired] = -65.0
            u[fired] = u[fired] + d[fired]

        # count neurons in group A an B
        if t in count_period:
            count_A += numpy.intersect1d(fired, A).size
            count_B += numpy.intersect1d(fired, B).size
            if t == count_period[-1]:
                if count_A > count_B:
                    print("Reward")
                    rew.append(t + random.randrange(0, 1000))

        # deliver spikes to post-synaptic neurons
        firings.appendleft(fired)
        for time in range(len(firings)):
            for fired_neuron in firings[time]:
                post_neurons = delays[fired_neuron[0]][time]
                if len(post_neurons) > 0:
                    I[post_neurons] += s[fired_neuron, post_neurons]

        # update post-synaptic potential
        for i in range(2):
            v += 0.5 * ((0.04 * v + 5.0) * v + 140.0 - u + I)
        u += a * (0.2 * v - u)

        # update eligibility trace
        STDP[fired] = 0.1
        for fired_neuron in fired:
            if fired_neuron < Ne:
                post_neurons = post[fired_neuron,:]
                sd[fired_neuron, post_neurons] -= 1.2 * STDP[post_neurons]
            pre_neurons = pre[fired_neuron[0]]
            sd[pre_neurons, fired_neuron] += STDP[pre_neurons]
        STDP *= 0.95

        # update synaptics weights
        DA *= 0.995
        if t % 10 == 0:
            s[0:Ne,:] = numpy.maximum(0.0, numpy.minimum(sm,
                s[0:Ne,:] + (0.002 + DA) * sd[0:Ne,:]))
            sd *= 0.99

        # issue reward
        if any([it == t for it in rew]):
            print("Increase dopamine")
            DA += 0.5

        # save plotting data
        data.firings.append(fired)

    # Update plots

    plt.subplot(211)

    points = []
    for ms in range(1000):
        t = sec * 1000 + ms
        for fired in data.firings[ms]:
            points.append((t, fired))

    # plot spikes
    scatter.set_offsets(points)
    plt.xlim(sec * 1000, sec * 1000 + 1000)
    plt.ylim(0, N)

    # plot mean value of synaptic strength

    s_ax = plt.subplot(212)

    data.s_A.append((sec + 1, numpy.sum(s[S, A])))
    data.s_B.append((sec + 1, numpy.sum(s[S, B])))
    line_A.set_data(
        [c[0] for c in data.s_A],
        [c[1] for c in data.s_A])
    line_B.set_data(
        [c[0] for c in data.s_B],
        [c[1] for c in data.s_B])

    s_ax.relim()
    s_ax.autoscale_view()

    # update UI
    plt.ion()
    plt.pause(0.01)
