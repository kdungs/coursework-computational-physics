#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

MARKERS = ['bs', 'r^', 'gv', 'm*']

for L, marker in zip(range(25, 101, 25), MARKERS):
    T, B, std = np.loadtxt('binder_{}.txt'.format(L), unpack=True)
    plt.errorbar(T, B, yerr=std, fmt=marker,
                 label='$L = {}$'.format(L))
plt.xlabel('$T$')
plt.ylabel('$B$')
plt.legend(loc='best')
plt.savefig('binder.pdf')
