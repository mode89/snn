import random

class Neuron:

    def __init__(self):
        self.potential = 0.0
        self.weight = [random.random() for i in range(10)]
        self.output = 0.0

    def integrate(self, neurons):
        for i in range(len(neurons)):
            self.potential += self.weight[i] * neurons[i].output

    def fire(self):
        if self.potential > 1.0:
            self.potential = 0.0
            self.output = 1.0
        else:
            self.output = 0.0

class Network:

    def __init__(self):
        self.neurons = [Neuron() for i in range(10)]

    def step(self):
        self.integrate()
        self.fire()

    def integrate(self):
        for neuron in self.neurons:
            neuron.integrate(self.neurons)

    def fire(self):
        for neuron in self.neurons:
            neuron.fire()
