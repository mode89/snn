import random
import numpy

class Network:

    def __init__(self, neuronNum):
        self.N = neuronNum
        self.Ne = int(self.N * 0.8)
        self.Ni = self.N - self.Ne

        re = numpy.random.rand(self.Ne)
        ri = numpy.random.rand(self.Ni)

        self.a = numpy.concatenate(
            (0.02 * numpy.ones(self.Ne), 0.02 + 0.08 * ri))
        self.b = numpy.concatenate(
            (0.2 * numpy.ones(self.Ne), 0.25 - 0.05 * ri))
        self.c = numpy.concatenate(
            (-65 + 15 * numpy.square(re), -65 * numpy.ones(self.Ni)))
        self.d = numpy.concatenate(
            (8 - 6 * numpy.square(re), 2 * numpy.ones(self.Ni)))

        self.S = numpy.concatenate((
            0.5 * numpy.random.rand(self.N, self.Ne),
            -1.0 * numpy.random.rand(self.N, self.Ni)),
            axis=1)

        self.v = -65 * numpy.ones(self.N)
        self.u = numpy.multiply(self.b, self.v)

    def update(self):
        I = numpy.concatenate((
            5 * numpy.random.randn(self.Ne),
            2 * numpy.random.randn(self.Ni)))
        self.fired = numpy.argwhere(self.v >= 30)
        print(self.fired)
        if self.fired.size > 0:
            self.v[self.fired] = self.c[self.fired]
            self.u[self.fired] += self.d[self.fired]
            I += numpy.sum(self.S[:, numpy.transpose(self.fired)[0]], 1)
        self.v += \
            0.04 * numpy.square(self.v) + 5 * self.v + 140 - self.u + I
        self.u += \
            numpy.multiply(self.a, numpy.multiply(self.b, self.v) - self.u)

class Monitor:

    def __init__(self, network):
        self.network = network
        self.counter = 0
        self.x = []
        self.y = []

    def update(self):
        for firing in self.network.fired:
            self.x.append(self.counter)
            self.y.append(firing[0])
        self.counter += 1
