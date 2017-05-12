import matplotlib.pyplot as plt
import numpy
import random
import snn

random.seed(0)

T = 1000
N = 1000
D = 20

network = snn.network(N, D)

firings_hist = []

for t in range(T):
    print(t)

    network.I[:] = numpy.zeros((N, 1))
    network.I[int(N * random.random())] = 20.0

    network.find_fired_neurons()
    network.reset_fired_neurons()
    network.deliver_delayed_spikes()
    network.update_potentials()

    fired = numpy.copy(network.fired)
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
