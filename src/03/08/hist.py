#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

# (a, b)
bm, cl = np.loadtxt('gaussian.txt', unpack=True)
plt.hist(bm, bins=100, normed=True, alpha=.5, label='Box-Mueller',
         linewidth=0)
plt.hist(cl, bins=100, normed=True, alpha=.5, label='Central Limit',
         linewidth=0)
xs = np.linspace(-4, 4, 10000)
plt.plot(xs, np.exp(-.5 * xs ** 2) / np.sqrt(2 * np.pi), 'r-')
plt.xlabel('$x$')
plt.ylabel('$p(x)$')
plt.legend(loc='best')
plt.savefig('gaussian.pdf')
plt.clf()

# (c)
ar = np.loadtxt('sinxover2.txt')
plt.hist(ar, bins=100, normed=True, alpha=.5, linewidth=0)
xs = np.linspace(0, np.pi, 10000)
plt.plot(xs, np.sin(xs) / 2, 'r-')
plt.xlabel('$x$')
plt.ylabel('$p(x)$')
plt.savefig('sinxover2.pdf')
plt.clf()

# (d)
ar = np.loadtxt('3xsquared.txt')
plt.hist(ar, bins=100, normed=True, alpha=.5, linewidth=0)
xs = np.linspace(0, 1, 10000)
plt.plot(xs, 3 * xs ** 2, 'r-')
plt.xlabel('$x$')
plt.ylabel('$p(x)$')
plt.savefig('3xsquared.pdf')
