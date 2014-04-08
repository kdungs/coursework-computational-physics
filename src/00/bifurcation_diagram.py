#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from os import listdir

files = listdir('./data/')
for f in files:
    r = float(f.replace('.txt', ''))
    xN = np.loadtxt('./data/{}'.format(f))
    plt.plot([r] * len(xN), xN, 'k.')
plt.xlabel('$r$')
plt.ylabel('$x_N$')
plt.minorticks_on()
plt.savefig('bifurcation_diagram.pdf')
