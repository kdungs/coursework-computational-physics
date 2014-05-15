#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

# calculate theoretical value
N = 100 * 100
beta = 1
Jx, Jy = 1, 0
lambda_p = np.exp(beta * Jx) + np.exp(-beta * Jx)
lambda_m = np.exp(beta * Jx) - np.exp(-beta * Jx)
E = -N * np.sinh(beta) / np.cosh(beta)
print(E)

# make nice plot
sc = np.loadtxt('decoupled.txt', unpack=True)
plt.pcolor(sc, cmap=plt.cm.copper)
plt.savefig('decoupled.png')
