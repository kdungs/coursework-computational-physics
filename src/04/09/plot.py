#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from scipy.special import gamma

def V(d):
    return np.pi ** (d / 2) / gamma(1 + d / 2)

d = np.arange(1, 201)
Qs = np.loadtxt('Qs.txt', unpack=True)
Vs = [2]
for i in range(0, 199):
    Vs.append(Qs[i] * 2 * Vs[i]);
plt.plot(d, Vs)
plt.plot(d, V(d))
plt.yscale('log')
plt.savefig('plot.pdf')
