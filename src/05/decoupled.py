#!/usr/bin/env python

import numpy as np

t, Eu, Eu_, Er, Er_ = np.loadtxt('decoupled.txt', unpack=True)

# theoretical
N = 100 * 100
beta = 1
Jx, Jy = 1, 0
lambda_p = np.exp(beta * Jx) + np.exp(-beta * Jx)
lambda_m = np.exp(beta * Jx) - np.exp(-beta * Jx)

E = -N * np.sinh(beta) / np.cosh(beta)

print(E)
print(Eu[-1])
print(Er[-1])
