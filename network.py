import random
import weakref

class Synapse:

    def output(self):
        return self.weight * self.pre().output

    def update_weight(self):
        if self.pre() is not self.post():
            if self.post().fired():
                self.weight += self.pre().spike_trace * 0.01
            elif self.pre().fired():
                self.weight -= self.post().spike_trace * 0.01

class Neuron:

    def __init__(self):
        self.spike_trace = 0.0

    def integrate(self):
        for synapse in self.synapses:
            self.potential += synapse.output()
        self.update_spike_trace()

    def update_spike_trace(self):
        if self.spike_trace > 0.0:
            self.spike_trace -= 0.1
        else:
            self.spike_trace = 0.0

    def fire(self):
        if self.potential > 1.0:
            self.potential = 0.0
            self.output = 1.0
            self.spike_trace = 1.0
        else:
            self.output = random.uniform(0.0, 1 / len(self.synapses))

    def update_weights(self):
        for synapse in self.synapses:
            synapse.update_weight()

    def fired(self):
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
                synapse.post = weakref.ref(neuron)
                synapse.weight = random.random() / neuron_num

    def step(self):
        self.integrate()
        self.fire()
        self.update_weights()

    def integrate(self):
        for neuron in self.neurons:
            neuron.integrate()

    def fire(self):
        for neuron in self.neurons:
            neuron.fire()

    def update_weights(self):
        for neuron in self.neurons:
            neuron.update_weights()

class Monitor:

    def __init__(self, network):
        self.network = network
        self.counter = 0
        self.x = list()
        self.y = list()

    def step(self):
        for i in range(len(self.network.neurons)):
            neuron = self.network.neurons[i]
            if neuron.fired():
                self.x.append(self.counter)
                self.y.append(i)
        self.counter += 1
