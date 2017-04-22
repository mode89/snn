import random

class Neuron:

    def __init__(self, input_num):
        self.input_num = input_num
        self.potential = random.random()
        self.weight = [random.random() / input_num
            for i in range(input_num)]
        self.output = 0.0

    def integrate(self, neurons):
        for i in range(len(neurons)):
            self.potential += self.weight[i] * neurons[i].output

    def fire(self):
        if self.potential > 1.0:
            self.potential = 0.0
            self.output = 1.0
        else:
            self.output = random.uniform(0.0, 1 / self.input_num)

    def spiking(self):
        return self.output == 1.0

class Network:

    def __init__(self, neuron_num):
        self.neurons = [Neuron(neuron_num) for i in range(neuron_num)]

    def step(self):
        self.integrate()
        self.fire()

    def integrate(self):
        for neuron in self.neurons:
            neuron.integrate(self.neurons)

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
