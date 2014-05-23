#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

beta = 1 / 8
nu = 1
Tc = 2 / np.log(1 + np.sqrt(2))
gM = 1

MARKERS = ['bs', 'r^', 'gv', 'm*']

for L, marker in zip(range(25, 101, 25), MARKERS):
    T, M, var = np.loadtxt('magnetisation_{}.txt'.format(L), unpack=True)
    plt.errorbar((T - Tc) / Tc, M, yerr=var, fmt=marker,
                 label='$L = {}$'.format(L))
plt.vlines([0], 0, 10000, color='k', linestyle='dashed')
plt.xlabel('$(T - T_c) / T_c$')
plt.ylabel('$M$')
plt.legend(loc='best')
plt.savefig('magnetisation.pdf')
