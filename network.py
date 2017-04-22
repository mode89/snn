import random
import weakref

class Synapse:

    def output(self):
        return self.weight * self.pre().output

class Neuron:

    def integrate(self):
        for synapse in self.synapses:
            self.potential += synapse.output()

    def fire(self):
        if self.potential > 1.0:
            self.potential = 0.0
            self.output = 1.0
        else:
            self.output = random.uniform(0.0, 1 / len(self.synapses))

    def spiking(self):
        return self.output == 1.0

class Network:

    def __init__(self, neuron_num):
        self.neurons = [Neuron() for i in range(neuron_num)]
        for neuron in self.neurons:
            neuron.potential = random.random()
            neuron.output = 0.0
            neuron.synapses = [Synapse() for i in range(neuron_num)]
            for synapse, pre_neuron in zip(neuron.synapses, self.neurons):
                synapse.pre = weakref.ref(pre_neuron)
                synapse.post = neuron
                synapse.weight = random.random() / neuron_num

    def step(self):
        self.integrate()
        self.fire()

    def integrate(self):
        for neuron in self.neurons:
            neuron.integrate()

    def fire(self):
        for neuron in self.neurons:
            neuron.fire()

class Monitor:

    def __init__(self, network):
        self.network = network
        self.counter = 0
        self.x = list()
        self.y = list()

    def step(self):
        for i in range(len(self.network.neurons)):
            neuron = self.network.neurons[i]
            if neuron.spiking():
                self.x.append(self.counter)
                self.y.append(i)
        self.counter += 1
