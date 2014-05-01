#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

# (a, b)
bm, cl = np.loadtxt('gaussian.txt', unpack=True)
plt.hist(bm, alpha=.5, label='Box-Mueller')
plt.hist(cl, alpha=.5, label='Central Limit')
plt.xlabel('$x$')
plt.ylabel('#')
plt.legend(loc='best')
plt.savefig('gaussian.pdf')
plt.clf()

# (c)
ar = np.loadtxt('sinxover2.txt')
plt.hist(ar, alpha=.5)
plt.xlabel('$x$')
plt.ylabel('#')
plt.savefig('sinxover2.pdf')
