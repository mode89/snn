import matplotlib.pyplot as plt
import snn

N = 1000

network = snn.network(N)

firings = [[], []]

for t in range(1000):
    network.generate_random_input()
    network.process_firings()
    network.update_potentials()
    if network.fired.size > 0:
        for firing in network.fired:
            firings[0].append(t)
            firings[1].append(firing)

plt.scatter(
    firings[0],
    firings[1],
    color="black",
    marker=".")
plt.show()
