import collections
import numpy
import random

EXITATORY_AMOUNT = 0.8
CONNECTIVITY = 0.1
MAX_DELAY = 20

class Network:

    def __init__(self, neuronNum):
        self.N = neuronNum
        self.Ne = int(self.N * EXITATORY_AMOUNT)
        self.Ni = self.N - self.Ne
        self.M = int(self.N * CONNECTIVITY)

        self.a = numpy.concatenate((
            0.02 * numpy.ones(self.Ne),
            0.1 * numpy.ones(self.Ni)
        ))
        self.b = numpy.concatenate((
            0.2 * numpy.ones(self.Ne),
            0.2 * numpy.ones(self.Ni)
        ))
        self.c = numpy.concatenate((
            -65 * numpy.ones(self.Ne),
            -65 * numpy.ones(self.Ni)
        ))
        self.d = numpy.concatenate((
            8 * numpy.ones(self.Ne),
            2 * numpy.ones(self.Ni)
        ))

        self.S = numpy.concatenate((
            6 * numpy.ones((self.Ne, self.M)),
            -5 * numpy.ones((self.Ni, self.M))
        ))

        self.v = -65 * numpy.ones(self.N)
        self.u = self.b * self.v

        self.post = numpy.empty((self.N, self.M), dtype=numpy.int)
        for i in range(self.Ne):
            self.post[i,:] = random.sample(range(self.N), self.M)
        for i in range(self.Ne, self.N):
            self.post[i,:] = random.sample(range(self.Ne), self.M)

        self.delays = \
            [[[] for i in range(MAX_DELAY)] for j in range(self.N)]
        for i in range(self.Ne):
            for j in range(self.M):
                self.delays[i][int(MAX_DELAY * random.random())].append(j)
        for i in range(self.Ne, self.N):
            self.delays[i][0] = range(self.M)

        self.firings = collections.deque(maxlen=MAX_DELAY)

    def update(self):
        I = numpy.zeros(self.N)
        I[int(self.N * random.random())] = 20.0

        self.fired = numpy.argwhere(self.v >= 30)
        if self.fired.size > 0:
            self.v[self.fired] = self.c[self.fired]
            self.u[self.fired] += self.d[self.fired]

        self.firings.appendleft(self.fired)
        for time in range(len(self.firings)):
            for fired in self.firings[time]:
                synapses = self.delays[fired][time]
                if len(synapses) > 0:
                    ind = self.post[fired, synapses]
                    I[ind] += self.S[fired, synapses]

        for i in range(2):
            self.v += 0.5 * (0.04 * numpy.square(self.v) + \
                5 * self.v + 140 - self.u + I)
        self.u += self.a * (self.b * self.v - self.u)

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
